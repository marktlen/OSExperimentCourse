#include "PluginController.h"
#include "PluginEnumerator.h"
#include <iostream>
#include <string.h>
#include <vector>

void printCommandsError(void); // 用于打印调用指令错误的内容

int main(int argc, char *argv[])
{
    //输入参数检查
    if (argc != 2)
    {
        printCommandsError();
        return 0;
    }

    //输入命令为help
    if (strcmp(argv[1], "help") == 0)
    {
        //实例化插件控制类
        PluginController pluginControl;
        pluginControl.ProcessHelp(); //调用插件的help函数

        return 0;
    }

    int functionID = atoi(argv[1]); // Convert string to number
    PluginController pluginControl; //实例化插件控制类

    if (functionID >= 0) // cheak input number in bounds
    {
        pluginControl.InitializeController();     // 初始化插件控制类
        pluginControl.ProcessRequest(functionID); // 请求调用对应ID的函数
        pluginControl.UninitializeController();   // 释放插件控制类占用
    }
    else //the input not allow
    {
        printCommandsError();
        return 0;
    }

    return 0;
}

/**
 * @brief 打印调用指令错误的内容
 * 
 */
void printCommandsError(void)
{
    std::cout << "Most used commands:" << std::endl;
    std::cout << "\thelp" << std::endl;
    std::cout << "\t[FunctionID]" << std::endl;
}