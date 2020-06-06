#include "2_1.h"
#include <dlfcn.h>
#include <iostream>

int main(void)
{
    // 延迟函数的调用绑定文件libPrint.so
    void *handle = dlopen("./libPrint.so", RTLD_LAZY);
    if (handle == NULL) //检测动态链接是否成功
    {
        std::cout << "dlopen error" << std::endl;
        return 0;
    }
    typedef void (*Fun)(); // 函数原型

    // 映射动态链接库中printStr的函数原型
    Fun funPrintStr = (Fun)dlsym(handle, "printStr");

    if (funPrintStr == 0)
    {
        std::cout << "Fun PrintStr error" << std::endl;
        std::cout << dlerror() << std::endl; //查看出错原因
    }

    (*funPrintStr)(); // 用函数指针调用函数

    dlclose(handle); // 关闭动态链接库

    return 0;
}