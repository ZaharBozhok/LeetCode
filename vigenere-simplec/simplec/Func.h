#ifndef FUNC_H
#define FUNC_H

#include <unordered_map>
#include <string>

class Func
{
    using Relations = std::unordered_map<std::string, Func *>;

public:
    Func(const std::string &name) : m_name(name) {}
    Func() = default;
    const std::string &Name() const { return m_name; }
    void AddCallsTo(Func &other)
    {
        AddRelationOrIgnore(other, m_callsTo);
    }
    void AddCalledBy(Func &other)
    {
        AddRelationOrIgnore(other, m_calledBy);
    }
    const Relations &CallsTo() const { return m_callsTo; }
    const Relations &CalledBy() const { return m_calledBy; }
    Relations &CallsTo() { return m_callsTo; }
    Relations &CalledBy() { return m_calledBy; }
    friend std::ostream &operator<<(std::ostream &os, const Func &f)
    {
        os << "Function `" << f.Name() << "` calls(" << f.CallsTo().size() << "), called by(" << f.CalledBy().size() << ")\n";
        if (!f.CalledBy().empty())
        {
            os << "\tCalled by : ";
            for (const auto &[name, _] : f.CalledBy())
                os << "`" << name << "`; ";
            os << '\n';
        }
        if (!f.CallsTo().empty())
        {
            os << "\tCalls to : ";
            for (const auto &[name, _] : f.CallsTo())
                os << "`" << name << "`; ";
            os << '\n';
        }
        return os;
    }

private:
    void AddRelationOrIgnore(Func &other, Relations &rels)
    {
        const auto &name = other.Name();
        if (rels.find(name) == rels.cend())
        {
            rels[name] = &other;
        }
    }

private:
    std::string m_name;
    Relations m_callsTo;
    Relations m_calledBy;
};

#endif // FUNC_H