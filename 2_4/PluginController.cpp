#include "PluginController.h"
#include "PluginEnumerator.h"
#include <dlfcn.h>
#include <iostream>

PluginController::PluginController()
{
}

PluginController::~PluginController()
{
}

/**
 * @brief 初始化插件控制类
 * 变量plugin插件目录，获取插件路径表和所以函数对应链接
 * @return true 
 * @return false 
 */
bool PluginController::InitializeController(void)
{
    //获取并存放插件
    std::vector<std::string> vstrPluginPath;

    PluginEnumerator pluginEnum;                      // 实例化插件枚举类
    if (!(pluginEnum.GetPluginNames(vstrPluginPath))) // 获取所以插件路径
    {
        return false;
    }
    int count = vstrPluginPath.size(); // 获取插件数量
    for (int i = 0; i < count; i++)
    {
        typedef void (*PLUGIN_CREATE)(IPrintPlugin **); //定义函数指针类型
        PLUGIN_CREATE createProc;                       //声明函数指针

        IPrintPlugin *pPlugin = NULL; //创建一个函数指针

        // 延迟函数的调用绑定插件目录下的文件
        void *pluginLib = dlopen(vstrPluginPath[i].c_str(), RTLD_LAZY);
        if (pluginLib != NULL) //判断是否成功绑定链接
        {
            m_vhForPlugin.push_back(pluginLib);                        // 保存动态链接
            createProc = (PLUGIN_CREATE)dlsym(pluginLib, "CreateFun"); // 获取创建功能函数链接，强转成函数指针类型

            if (createProc != NULL) // 判断链接是否有效
            {
                (createProc)(&pPlugin); //创建插件对象
                /* 保存库函数对象 */
                if (pPlugin != NULL)
                {
                    m_vpPlugin.push_back(pPlugin); //保存创建对象的指针
                }
            }
        }
    }
    return true;
}

/**
 * @brief 查找对应ID并调用函数
 * 
 * @param FunctionID 函数ID
 * @return true 执行成功
 * @return false 无法找到对应ID
 */
bool PluginController::ProcessRequest(int FunctionID)
{
    int count = m_vpPlugin.size();
    int i;
    if (FunctionID > count)
    {
        /* 找不到对应ID */
        std::cout << "could't find function " << FunctionID << std::endl;
        return false;
    }

    for (i = 0; i < count; i++)
    {
        /* 查找对应ID的函数 */
        if (m_vpPlugin[i]->GetID() == FunctionID)
        {
            /* 查询后调用 */
            m_vpPlugin[i]->PrintStr();
            break;
        }
    }

    return true;
}
/**
 * @brief 逐一调用pulgin目录里的help函数
 * 
 * @return true 成功调用
 * @return false 获取路径错误
 */
bool PluginController::ProcessHelp(void)
{
    //获取并存放插件
    std::vector<std::string> vstrPluginPath;
    PluginEnumerator pluginEnum;                      // 实例化插件枚举类
    if (!(pluginEnum.GetPluginNames(vstrPluginPath))) // 获取所以插件路径
    {
        std::cout << "could't get plugin paths " << std::endl;
        return false;
    }
    int count = vstrPluginPath.size(); // 获取插件数量
    for (int i = 0; i < count; i++)
    {
        typedef void (*PLUGIN_CREATE)(IPrintPlugin **); //定义函数指针类型
        PLUGIN_CREATE createProc;                       //声明函数指针

        IPrintPlugin *pPlugin = NULL; //创建一个函数指针

        // 逐一调用插件里的help函数
        void *pluginLib = dlopen(vstrPluginPath[i].c_str(), RTLD_LAZY);
        if (pluginLib != NULL) //判断是否成功绑定链接
        {
            m_vhForPlugin.push_back(pluginLib);                        // 保存动态链接
            createProc = (PLUGIN_CREATE)dlsym(pluginLib, "CreateFun"); // 获取创建功能函数链接，强转成函数指针类型

            if (createProc != NULL) // 判断链接是否有效
            {
                (createProc)(&pPlugin); //创建插件对象
                /* 保存库函数对象 */
                if (pPlugin != NULL)
                {
                    pPlugin->Help();//调用函数
                }
            }
            dlclose(pluginLib);
        }
    }
    return true;
}

/**
 * @brief 逐一关闭链接，释放空间
 * 
 * @return true 
 * @return false 
 */
bool PluginController::UninitializeController(void)
{
    int count = m_vhForPlugin.size(); // 获取插件数量
    for (int i = 0; i < count; i++)
    {
        dlclose(m_vhForPlugin[i]);
    }
    return true;
}