#include <iostream>

const int FUNC_ID = 1;

extern "C" void printStr()
{
    std::cout << "Hello China" << std::endl;
}

extern "C" void help()
{
    std::cout << "Function ID:"<< FUNC_ID << "\tThis fucntion print \"Hello China\"" << std::endl;
}