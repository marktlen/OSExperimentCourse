#include "PluginEnumerator.h"
#include <dirent.h>
#include <dlfcn.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

#define MAX_D_NAME_LENGTH 256 /*最大路径字符长度*/

bool dlFunctionHook(string path, char *op);

int main(int argc, char *argv[])
{
    if (argc != 2) //需要给出调用参数
    {
        std::cout << "Most used commands:" << std::endl;
        std::cout << "\thelp" << std::endl;
        std::cout << "\t[FuncID]" << std::endl;
        return 0;
    }

    //vector of plugin path
    std::vector<std::string> strPluginPath;

    //get plugin path
    PluginEnumerator pluginEnum;
    if (!pluginEnum.GetPluginNames(strPluginPath))
    {
        std::cout << "Get plugin names error" << std::endl;
        return 0;
    }

    int num_argv1 = atoi(argv[1]);            // Convert string to number
    int strPluginsize = strPluginPath.size(); // plugin number

    if (strcmp(argv[1], "help") == 0) // if input help
    {
        //Cycle call help() in all plugin
        for (int i = 0; i < strPluginsize; i++)
        {
            dlFunctionHook(strPluginPath[i], (char *)"help");
        }
    }
    else if (num_argv1 >= 0 && num_argv1 <= strPluginsize)// cheak input number in bounds
    {
        dlFunctionHook(strPluginPath[num_argv1], (char *)"printStr");//use funcID select function
    }
    else //the input not allow
    {
        std::cout << "need follow operation:" << std::endl;
        std::cout << "\thelp" << std::endl;
        std::cout << "\t[FuncID]" << std::endl;
        return 0;
    }

    return 0;
}

/**
 * @brief call plugin
 * 
 * @param path path of the dynamic link file
 * @param op function name
 * @return true success
 * @return false fail
 */
bool dlFunctionHook(string path, char *op)
{
    //延迟函数的调用绑定插件目录下的文件
    void *handle = dlopen(path.c_str(), RTLD_LAZY);

    if (handle == NULL) //检测动态链接是否成功
    {
        std::cout << "dlopen error" << std::endl;
        return false;
    }
    typedef void (*Fun)(); // 函数原型

    // 映射动态链接库中printStr的函数原型
    Fun funPrintStr = (Fun)dlsym(handle, op);

    if (funPrintStr == NULL)
    {
        std::cout << "Fun " << op << " error" << std::endl;
        std::cout << dlerror() << std::endl; //查看出错原因
        return false;
    }

    (*funPrintStr)(); // 用函数指针调用函数

    dlclose(handle); // 关闭动态链接库
    return true;
}
