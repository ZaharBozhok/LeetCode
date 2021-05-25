#include <iostream>
#include <fstream>
#include <sstream>
#include "Func.h"
#include "FuncStorage.h"

FuncStorage::FuncCalls GetFuncCallsFromFile(const std::string &path)
{
    std::ifstream file;
    file.open(path);
    if (!file)
        throw std::runtime_error("Failed to open " + path);
    std::string caller, calls, callee;
    FuncStorage::FuncCalls funcCalls;
    while (file >> caller >> calls >> callee)
    {
        funcCalls.push_back({caller, callee});
    }
    return funcCalls;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Provide path to .in file\n";
        return -1;
    }
    try
    {
        std::string pathIn = argv[1];
        auto funcCalls = GetFuncCallsFromFile(pathIn);
        FuncStorage funcStorage(funcCalls);
        auto impossible = funcStorage.HasCircularDependency();
        if (impossible == true)
        {
            std::cout << pathIn << " : impossible\n";
            return -1;
        }
        funcStorage.PrintDeclarationsAndUnchainAll(std::cout);
    }
    catch (const std::exception &e)
    {
        std::cerr << '\t' << e.what() << '\n';
        return -1;
    }
    return 0;
}
