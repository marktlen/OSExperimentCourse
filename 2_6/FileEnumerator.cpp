#include "FileEnumerator.h"
#include <dirent.h>
#include <iostream>
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_D_NAME_LENGTH 256 /*最大路径字符长度*/

FileEnumerator::FileEnumerator(/* args */)
{
}
FileEnumerator::~FileEnumerator()
{
}

/**
 * @brief get path of files, 
 * 
 * @param path diretectory path
 * @param vfilePaths files path
 * @return int number of files
 */
int FileEnumerator::FileEnum(char *path)
{
    int counter = 0;
    DIR *dir;                          //目录流对象的数据类型指针
    struct stat stat_buf;              //文件属性
    struct dirent *ptr;                //目录项结构体指针
    char curr_path[MAX_D_NAME_LENGTH]; //扫描的当前目录项，方便打开目录
    dir = opendir(path);               //打开当前文件夹

    /*文件路径错误判断*/
    if (dir == NULL)
    {
        std::cout << "can't open this directory" << std::endl;
        return false;
    }

    // 循环读取当前文件夹内的目录项
    while ((ptr = readdir(dir)) != NULL)
    {
        if (ptr->d_name[0] == '.')
        {
            //剔除隐藏文件和文件自身，保证文件隐藏和打印效果
            continue;
        }

        stat(ptr->d_name, &stat_buf);              // 获取文件信息，把信息放到s_buf中
        memcpy(curr_path, path, strlen(path) + 1); // 获取当前文件路径

        /* 手动处理文件夹路径 */
        strcat(curr_path, "/");
        strcat(curr_path, ptr->d_name);

        //存储文件路径
        vfilePaths.push_back(curr_path);

        counter++;
    }
    closedir(dir); //关闭当前文件夹，节省资源
    return true;
}

void FileEnumerator::showFileList()
{
    DIR *dir;             //目录流对象的数据类型指针
    struct stat stat_buf; //文件属性
    int count = vfilePaths.size();
    // 循环读取当前文件夹内的目录项
    for (int i = 0; i < count; i++)
    {
        stat(vfilePaths[i].c_str(), &stat_buf); // 获取文件信息，把信息放到s_buf中
        std::cout
            << "File ID:" << i
            << '\t' << stat_buf.st_size
            << '\t' << basename((char *)vfilePaths[i].c_str()) << std::endl; //打印当前文件
    }
}