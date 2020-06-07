#include "PluginEnumerator.h"
#include <dirent.h>
#include <iostream>
#include <string>

#define MAX_D_NAME_LENGTH 256 /*最大路径字符长度*/

PluginEnumerator::PluginEnumerator(/* args */)
{
}
PluginEnumerator::~PluginEnumerator()
{
}

/**
 * @brief get plugin path in plugin directory
 * 
 * @param vStrPlugin save plugin path
 * @return true 
 * @return false 
 */
bool PluginEnumerator::GetPluginNames(vector<string> &vStrPlugin)
{
    DIR *dir;                          //目录流对象的数据类型指针
    struct dirent *ptr;                //目录项结构体指针
    char *path = (char *)"./plugin";   //open directory plugin
    //char *curr_path;//[MAX_D_NAME_LENGTH]; // current directory
    dir = opendir(path); //open directory

    // chack if the directory exists
    if (dir == NULL)
    {
        cout << "There's not exist directory called plugin" << endl;
        return false;
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        if (ptr->d_name[0] == '.')
        {
            //剔除隐藏文件和文件自身，保证文件隐藏和打印效果
            continue;
        }
        /* 手动处理文件夹路径 */
        string str = "./plugin/";
        str += ptr->d_name;

        vStrPlugin.push_back(str);
    }
    closedir(dir);
    return true;
}
