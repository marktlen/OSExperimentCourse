#include "PluginController.h"
#include "PluginEnumerator.h"
#include <dlfcn.h>
#include <iostream>

PluginController::PluginController(/* args */)
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
        // 延迟函数的调用绑定插件目录下的文件
        void *pluginLib = dlopen(vstrPluginPath[i].c_str(), RTLD_LAZY);
        if (pluginLib != NULL) //判断是否成功绑定链接
        {
            m_vhForPlugin.push_back(pluginLib); // 保存动态链接

            // 存储两个函数链接
            PROC_PRINTSTR dllPrint = (PROC_PRINTSTR)dlsym(pluginLib, "PrintStr"); // 获取打印函数链接
            PROC_GETID dllGetID = (PROC_GETID)dlsym(pluginLib, "GetID");          // 获取获得ID函数链接
            if ((dllPrint != NULL) && (dllGetID != NULL))                         // 判断链接是否有效
            {
                /* 保存两个函数的链接 */
                m_vPrintStrFunc.push_back(dllPrint);
                m_vGetIDFunc.push_back(dllGetID);
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
    int count = m_vhForPlugin.size();
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
        if ((m_vGetIDFunc[i])() == FunctionID)
        {
            /* 查询后调用 */
            (m_vPrintStrFunc[i]());
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
        // 逐一调用插件里的help函数
        if (!dlFunctionHook(vstrPluginPath[i].c_str(), (char *)"Help"))
        {
            std::cout << "could't find function help" << std::endl;
        }
    }
    return true;
}

/**
 * @brief call plugin function hook
 * 
 * @param path path of the dynamic link file
 * @param op function name
 * @return true success
 * @return false fail
 */
bool PluginController::dlFunctionHook(string path, char *op)
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