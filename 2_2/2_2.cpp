#include <dirent.h>
#include <dlfcn.h>
#include <iostream>
#include <string.h>
#include <vector>

#define MAX_D_NAME_LENGTH 256 /*最大路径字符长度*/

int main(void)
{

    DIR *dir;                          //目录流对象的数据类型指针
    struct dirent *ptr;                //目录项结构体指针
    char *path = (char *)"./plugin";   //open directory plugin
    char curr_path[MAX_D_NAME_LENGTH]; // current directory

    dir = opendir(path); //open directory

    // chack if the directory exists
    if (dir == NULL)
    {
        std::cout << "There's not exist directory called plugin" << std::endl;
    }

    while ((ptr = readdir(dir)) != NULL) //循环遍历，直到目录项为空
    {
        if (ptr->d_name[0] == '.')
        {
            //剔除隐藏文件和文件自身，保证文件隐藏和打印效果
            continue;
        }
        /* 手动处理文件夹路径 */
        memcpy(curr_path, path, strlen(path) + 1); // 获取当前文件路径
        strcat(curr_path, "/");                    // 加斜杠
        strcat(curr_path, ptr->d_name);            // 加文件名

        //延迟函数的调用绑定插件目录下的文件
        void *handle = dlopen(curr_path, RTLD_LAZY);

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
    }
    closedir(dir);
    return 0;
}