#ifndef CPLUGINCONTROLLER_H
#define CPLUGINCONTROLLER_H

#include <vector>
#include <string>

//定义三种操作函数的指针对应类型
typedef void (*PROC_PRINTSTR)(void);
typedef void (*PROC_HELP)(void);
typedef int (*PROC_GETID)(void);

class PluginController
{
private:
    std::vector<void *> m_vhForPlugin;          //plugin的句柄
    std::vector<PROC_PRINTSTR> m_vPrintStrFunc; //打印函数指针
    std::vector<PROC_GETID> m_vGetIDFunc;       //获取函数指针

public:
    PluginController(/* args */);
    ~PluginController();

    bool InitializeController(void);   //初始化控制类
    bool UninitializeController(void); //反初始化控制类

    bool dlFunctionHook(std::string path, char *op); // 插件函数调用钩子

    bool ProcessHelp(void);              //调用Help函数|
    bool ProcessRequest(int FunctionID); //调用对应FUNID的函数
};

#endif