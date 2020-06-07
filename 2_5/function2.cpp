#include <iostream>
#include "IPrintPlugin.h"

const int FUNC_ID = 2;

class CPrintPlugin : public IPrintPlugin
{
public:
    CPrintPlugin(/* args */);
    virtual ~CPrintPlugin();

    virtual void PrintStr()//打印函数
    {
        std::cout << "Hollw China!" << std::endl;
    }

    virtual void Help()//帮助函数
    {
        std::cout << "Function ID:"<< FUNC_ID << "\tThis fucntion print \"Hello China\"" << std::endl;
    }
    virtual int GetID()//获取函数ID
    {
        return FUNC_ID;
    }
};

CPrintPlugin::CPrintPlugin(/* args */)
{
}

CPrintPlugin::~CPrintPlugin()
{
}

/**
 * @brief 调用库之前，调用此函数创建一个对象
 * 给对象指针所对应的值赋值为新创建的对象
 */
extern "C" void CreateFun(IPrintPlugin **ppPlugin)
{
    static CPrintPlugin plugin; // 静态保证了加载时实例化对象
    *ppPlugin = &plugin;    // 将实例化对象赋给对象指针
}