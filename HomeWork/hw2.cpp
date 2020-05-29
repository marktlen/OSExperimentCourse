/**
 * TODO 当文件夹第一个文件（无后缀名）与文件夹同名时，会产生连续性的无法打开目录的错误
 */
#include <dirent.h>
#include <iostream>
#include <string.h>
#include <sys/stat.h>

#define RECURSION_DEPTH 3     /*定义递归深度*/
#define MAX_D_NAME_LENGTH 256 /*最大路径字符长度*/

/**
 * @brief 文件路径递归调用
 * 
 * @param path 需要读取的文件夹路径
 * @param depth 递归深度
 * @return true 成功
 * @return false 文件未知类型错误,文件路径无法打开
 */
bool Recursive(char *path, int depth)
{
    /*设置递归深度，深度达到 RECURISON_DEPTH时不进行文件扫描*/
    if (depth >= RECURSION_DEPTH)
    {
        std::cout << "..." << std::endl;
        return true;
    }
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

        stat(ptr->d_name, &stat_buf);          // 获取文件信息，把信息放到s_buf中
        memcpy(curr_path, path, sizeof(path)); // 获取当前文件路径
        /* 判断是不是文件夹 */
        if (S_ISDIR(stat_buf.st_mode)) //是一个文件夹
        {
            /* 手动处理文件夹路径 */
            strcat(curr_path, "/");
            strcat(curr_path, ptr->d_name);

            std::cout << curr_path << std::endl; // 打印当前文件夹名
            Recursive(curr_path, depth + 1);     // 递归当前文件夹，深度加1
        }
        else if (!S_ISDIR(stat_buf.st_mode) == 0) //不是个文件夹
        {
            // 为了美化打印效果打印效果，增强层次感
            if (depth >= 1)
            {
                printf(" ");
                for (int i = 0; i < depth; i++)
                {
                    printf("-");
                }
                printf("|");
            }

            std::cout << ptr->d_name << std::endl; //打印当前文件
        }
        else
        {
            std::cout << "file mode error!" << std::endl;
            return false;
        }
    }
    closedir(dir); //关闭当前文件夹，节省资源
    return true;
}

int main(int argc, char *argv[])
{
    // 规定只填入路径作为参数
    if (argc != 2)
    {
        std::cout << "pleas input the file path to be printed." << std::endl;
        return -1;
    }

    /*传入的第一个参数作为路径*/
    char *path = argv[1];
    path = argv[1];

    int depth = 0;                 //初始化当前递归深度
    return Recursive(path, depth); //开始文件夹递归打印
}