#include "FileEnumerator.h"
#include "IFileCounterPlugin.h"
#include <iostream>
#include <sys/stat.h>

const int FUNC_ID = 1;

class CFilePlugin : public IFileCounterPlugin
{
private:
    /* data */
public:
    CFilePlugin(/* args */);
    virtual ~CFilePlugin();

    virtual bool FileInf(const char *path) //统计函数
    {
        int flag, count = 0;   //文件行数统计
        struct stat stat_buf;  //文件信息接收结构
        stat(path, &stat_buf); //文件信息获取
        FILE *fp;              //文件指针
        fp = fopen(path, "r"); //只读打开文件
        if (fp == NULL)
        {
            std::cout << "open error!" << std::endl;
            return false;
        }
        while (!feof(fp))
        {
            flag = fgetc(fp);
            if (flag == '\n')
                count++;
        }
        std::cout
            << "line number: " << count
            << "\tfile size:" << stat_buf.st_size << std::endl; //打印当前文件

        return true;
    }

    virtual void Help() //帮助函数
    {
        std::cout << "Function ID:" << FUNC_ID << "\tThis fucntion is line number and file size counter" << std::endl;
    }

    virtual int GetID() //获取函数ID
    {
        return FUNC_ID;
    }
};

CFilePlugin::CFilePlugin(/* args */)
{
}

CFilePlugin::~CFilePlugin()
{
}
/**
 * @brief 调用库之前，调用此函数创建一个对象
 * 给对象指针所对应的值赋值为新创建的对象
 */
extern "C" void CreateCounter(IFileCounterPlugin **ppPlugin)
{
    static CFilePlugin plugin; // 静态保证了加载时实例化对象
    *ppPlugin = &plugin;       // 将实例化对象赋给对象指针
}