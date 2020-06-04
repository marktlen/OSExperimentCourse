#include <iostream>

const int FUNC_ID = 0;

extern "C" void printStr()
{
    std::cout << "Hello World" << std::endl;
}

extern "C" void help()
{
    std::cout << "Function ID:"<< FUNC_ID << "\tThis fucntion print \"Hello World\"" << std::endl;
}