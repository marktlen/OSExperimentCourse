#ifndef CPLUGINCONTROLLER_H
#define CPLUGINCONTROLLER_H

#include "IFileCounterPlugin.h"
#include <vector>

class PluginController
{
private:
    std::vector<void *> m_vhForPlugin;      //plugin的句柄
    std::vector<IFileCounterPlugin *> m_vpPlugin; //实例化的动态接口指针

public:
    PluginController(/* args */);
    ~PluginController();

    bool InitializeController(void);   //初始化控制类
    bool UninitializeController(void); //反初始化控制类

    bool ProcessHelp(void);              //调用Help函数
    bool ProcessRequest(int FunctionID,const char *path); //调用对应FUNID的函数
};

#endif