#ifndef FUNCSTORAGE_H
#define FUNCSTORAGE_H

#include <unordered_set>
#include <vector>
#include "Func.h"

class FuncStorage
{
public:
    struct FuncCall /* caller calls callee */
    {
        std::string caller;
        std::string callee;
    };
    using FuncCalls = std::vector<FuncCall>;

public:
    FuncStorage(const FuncCalls &funcCalls)
    {
        for (const auto &funcCall : funcCalls)
        {
            auto &callerFunc = GetOrCreateFunc(funcCall.caller);
            auto &calleeFunc = GetOrCreateFunc(funcCall.callee);
            ChainCallerAndCallee(callerFunc, calleeFunc);
            int i = 0;
        }
        GenerateCCode();
    }
    bool HasCircularDependency()
    {
        std::unordered_set<std::string> visited;
        for (const auto &func : m_funcsStorage)
        {
            try
            {
                HasCircularDependencyHelper(visited, func.second);
            }
            catch (const std::exception &e)
            {
                return true;
            }
        }
        return false;
    }
    void PrintDeclarationsAndUnchainAll(std::ostream &os)
    {
        while (m_funcsStorage.size())
        {
            UnchainEdgeFuncs(os);
        }
    }
    friend std::ostream &operator<<(std::ostream &os, const FuncStorage &funcStorage)
    {
        int i = 1;
        for (const auto &[name, func] : funcStorage.m_funcsStorage)
        {
            os << "[" << (i++) << "] " << func;
        }
        return os;
    }
    const std::unordered_map<std::string, std::string> &GeneratedCCode() const { return m_cFuncs; }

private:
    void GenerateCCode()
    {
        for (const auto &[funcName, func] : m_funcsStorage)
        {
            std::stringstream cFunc;
            cFunc << "void "
                  << "my_" << funcName << "()\n{\n";
            for (const auto &[calleeName, _] : func.CallsTo())
            {
                cFunc << '\t' << "my_" << calleeName << "();\n";
            }
            cFunc << "\n}";
            m_cFuncs[funcName] = cFunc.str();
        }
    }
    void UnchainEdgeFuncs(std::ostream &os)
    {
        std::vector<std::string> funcsToDrop;
        for (auto &[name, func] : m_funcsStorage)
        {
            if (func.CallsTo().size() == 0)
            {
                UnchainCalleeAndAllCallers(func);
                os << m_cFuncs[name] << '\n';
                funcsToDrop.push_back(name);
            }
        }
        for (const auto &name : funcsToDrop)
        {
            m_funcsStorage.erase(name);
        }
    }
    void UnchainCalleeAndAllCallers(Func &callee)
    {
        auto &callers = callee.CalledBy();
        for (auto &[callerName, callerFunc] : callers)
        {
            callerFunc->CallsTo().erase(callee.Name());
            int a = 0;
        }
        callers.clear();
    }
    void HasCircularDependencyHelper(std::unordered_set<std::string> visited, const Func &func)
    {
        auto it = visited.insert(func.Name());
        if (it.second == false)
        {
            throw std::runtime_error("impossible");
        }
        for (const auto &callee : func.CallsTo())
        {
            HasCircularDependencyHelper(visited, *callee.second);
        }
        visited.erase(it.first);
    }
    Func &GetOrCreateFunc(const std::string &name)
    {
        if (auto it = m_funcsStorage.find(name); it != m_funcsStorage.cend())
        {
            return it->second;
        }
        return (m_funcsStorage.insert({name, Func(name)}))
            .first->second;
    }
    void ChainCallerAndCallee(Func &caller, Func &callee)
    {
        caller.AddCallsTo(callee);
        callee.AddCalledBy(caller);
    }

private:
    std::unordered_map<std::string, Func> m_funcsStorage;
    std::unordered_map<std::string, std::string> m_cFuncs;
};


#endif //FUNCSTORAGE_H