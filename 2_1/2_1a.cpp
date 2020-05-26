#include <iostream>
#include "2_1.h"

/**
 * @brief 打印"hello China"
 * extern "C"保证导出的函数名称和实际名称一致
 */
extern "C" void printStr(void)
{
    std::cout << "Hello China" << std::endl;
}