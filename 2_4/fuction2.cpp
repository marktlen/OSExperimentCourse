#include <iostream>

const int FUNC_ID = 2;

extern "C" void PrintStr()
{
    std::cout << "Hollw China!" << std::endl;
}

extern "C" void Help()
{
    std::cout << "Function ID:"<< FUNC_ID << "\tThis fucntion print \"Hello China\"" << std::endl;
}

extern "C" int GetID()
{
    return FUNC_ID;
}