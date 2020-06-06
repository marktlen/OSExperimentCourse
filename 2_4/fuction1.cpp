#include <iostream>

const int FUNC_ID = 1;

extern "C" void PrintStr()
{
    std::cout << "Hollw World!" << std::endl;
}

extern "C" void Help()
{
    std::cout << "Function ID:"<< FUNC_ID << "\tThis fucntion print \"Hello World\"" << std::endl;
}

extern "C" int GetID()
{
    return FUNC_ID;
}