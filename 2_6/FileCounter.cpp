#include "FileEnumerator.h"
#include "PluginController.h"
#include "PluginEnumerator.h"
#include <dirent.h>
#include <grp.h>
#include <iostream>
#include <pwd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void printCommandsError(void);

int main(int argc, char *argv[])
{

    //输入参数检查
    if (argc < 2)
    {
        printCommandsError();
        return 0;
    }
    if (argc == 2)
    {
        /* 当输入一个参数时 */

        //输入命令为help
        if (strcmp(argv[1], "help") == 0)
        {
            printf("inter help");
            //实例化插件控制类
            PluginController pluginControl;
            pluginControl.ProcessHelp(); //调用插件的help函数

            return 0;
        }
        else
        {
            /*传入的第一个参数作为路径*/
            char *path = argv[1];
            path = argv[1];

            PluginController pluginControl;       //实例化插件控制类
            pluginControl.InitializeController(); //初始化插件控制类
            FileEnumerator fileEnum;              //实例化文件枚举类
            std::vector<std::string> vfilePaths;  //存储文件路径
            fileEnum.FileEnum(path);              //获取命令path下的所有文件
            fileEnum.showFileList();              //打印文件对应ID
        }
    }
    else if (argc == 4)
    {
        /* 输入两个参数，当做输入 操作函数ID 和 文件ID*/
        int functionID = atoi(argv[2]); // Convert string to number
        int fileID = atoi(argv[3]);     // file ID

        PluginController pluginControl;       //实例化插件控制类
        pluginControl.InitializeController(); //初始化插件控制类
        FileEnumerator fileEnum;              //实例化文件枚举类

        /*传入的第一个参数作为路径*/
        char *path = argv[1];
        path = argv[1];
        fileEnum.FileEnum(path); //获取命令path下的所有文件
        fileEnum.showFileList();              //打印文件对应ID

        if (functionID >= 0 && fileID >= 0) // cheak input number in bounds
        {
            //printf("pass!");
            //std::cout << fileEnum.vfilePaths[0] << std::endl;
            pluginControl.ProcessRequest(functionID, fileEnum.vfilePaths[fileID].c_str()); // 请求调用对应ID的函数
            pluginControl.UninitializeController();                                        // 释放插件控制类占用
        }
        // else //the input not allow
        // {
        //     printCommandsError();
        // }
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
    std::cout << "\tDir path" << std::endl;
    std::cout << "\tDir path [FunctionID] [FileID]" << std::endl;
}