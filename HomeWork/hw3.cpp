/**
 * 实现“ls -l”的基本功能，至少能输出文件类型、9个权限位信息、文件大小、文件名称
 * 
 * first line: the total space occupied of file 
 * Field: file attribute 10 words
 *      1:  File type: '-' ordinary file, 'd' dirtectory, 'l' link, 'b' block, 'c' character, 'p' command pipeline
 *          File permission: r read, w write, x execute
 *      2:  If isn't a dirtectory, Number of hard links; Is a directory, Number of subdirtectories
 *      3:  file owner
 *      4:  file owner group
 *      5:  file size (bits)
 *      6:  Last visited (modified) time. MM-DD mm:ss
 *      7:  file name
 * 
 * struct stat
 * {
 * dev_t       st_dev;   ID of device containing file -文件所在设备的ID
 * ino_t st_ino;         inode number -inode节点号
 * mode_t st_mode;       File mode 
 * nlink_t st_nlink;     number of hard links -链向此文件的连接数(硬连接)
 * uid_t st_uid;         user ID of owner -user id
 * gid_t st_gid;         group ID of owner - group id
 * dev_t st_rdev;        device ID (if special file) -设备号，针对设备文件
 * off_t st_size;        total size, in bytes -文件大小，字节为单位
 * blksize_t st_blksize; blocksize for filesystem I/O -系统块的大小
 * blkcnt_t st_blocks;   number of blocks allocated -文件所占块数
 * time_t st_atime;      time of last access -最近存取时间
 * time_t st_mtime;      time of last modification -最近修改时间
 * time_t st_ctime;      time of last status change - 文件状态改变时间
 * }
 *
 */
#include <dirent.h>
#include <grp.h>
#include <iostream>
#include <pwd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define RECURSION_DEPTH 3             /*定义递归深度*/
#define MAX_D_NAME_LENGTH 256         /*最大路径字符长度*/
#define yearsecs (60 * 60 * 24 * 365) /*seconds in a year*/

bool showDirecoryList(char *path);
char *getFileModeFlag(int mode); //get char file mode
char *uidToName(uid_t uid);
char *gidToName(gid_t gid);
int counterSubdirtectory(char *path);
int fileCounter(char *path);

/**
 * @brief 文件路径递归调用
 * 
 * @param path 需要读取的文件夹路径
 * @param depth 递归深度
 * @return true 成功
 * @return false 文件未知类型错误,文件路径无法打开
 */
bool showDirecoryList(char *path)
{
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
    std::cout << "count:" << fileCounter(path) << std::endl;
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

            std::cout
                << getFileModeFlag(stat_buf.st_mode)
                << ' ' << counterSubdirtectory(curr_path) //number of subdirtectories
                << ' ' << uidToName(stat_buf.st_uid)
                << ' ' << gidToName(stat_buf.st_gid)
                << ' ' << stat_buf.st_size
                << '\t' << (stat_buf.st_mtim.tv_sec / yearsecs) + 1970
                << ' ' << ptr->d_name << std::endl; //打印当前文件
        }
        else if (!S_ISDIR(stat_buf.st_mode)) // 不是个文件夹
        {
            std::cout
                << getFileModeFlag(stat_buf.st_mode)
                << ' ' << stat_buf.st_nlink
                << ' ' << uidToName(stat_buf.st_uid)
                << ' ' << gidToName(stat_buf.st_gid)
                << ' ' << stat_buf.st_size
                << '\t' << (stat_buf.st_mtim.tv_sec / yearsecs) + 1970
                << ' ' << ptr->d_name << std::endl; //打印当前文件
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

int fileCounter(char *path)
{
    int count = 0;
    DIR *dir;            //目录流对象的数据类型指针
    struct dirent *ptr;  //目录项结构体指针开目录
    dir = opendir(path); //打开当前文件夹

    while ((ptr = readdir(dir)) != NULL)
        count++;
    return count;
}
/**
 * @brief get number of subdirtectory
 * 
 * @param path diretectory path
 * @return number of subdirtectory
 */
int counterSubdirtectory(char *path)
{
    int counter = 1;
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
            counter++;
        }
    }
    closedir(dir); //关闭当前文件夹，节省资源
    return counter;
}

/**
 * @brief Get the File Mode Flag object
 * 
 * @param mode file mode
 * @return Flag char 
 */
char *getFileModeFlag(int mode)
{
    static char ptr[11] = "----------"; //Field: file attribute 10 words

    // File type
    if (S_ISREG(mode))
        ptr[0] = '-';
    else if (S_ISDIR(mode))
        ptr[0] = 'd';
    else if (S_ISCHR(mode))
        ptr[0] = 'c';
    else if (S_ISBLK(mode))
        ptr[0] = 'b';
    else if (S_ISFIFO(mode))
        ptr[0] = 'f';
    else if (S_ISLNK(mode))
        ptr[0] = 'l';
    else if (S_ISSOCK(mode))
        ptr[0] = 's';
    else
        ptr[0] = '?';

    //print field of file attribute
    if (mode & S_IRUSR)
    {
        ptr[1] = 'r';
    }
    if (mode & S_IWUSR)
    {
        ptr[2] = 'w';
    }
    if (mode & S_IXUSR)
    {
        ptr[3] = 'x';
    }
    if (mode & S_IRGRP)
    {
        ptr[4] = 'r';
    }
    if (mode & S_IWGRP)
    {
        ptr[5] = 'w';
    }
    if (mode & S_IXGRP)
    {
        ptr[6] = 'x';
    }
    if (mode & S_IROTH)
    {
        ptr[7] = 'r';
    }
    if (mode & S_IWOTH)
    {
        ptr[8] = 'w';
    }
    if (mode & S_IXOTH)
    {
        ptr[9] = 'x';
    }

    return (char *)ptr;
}

/**
 * @brief Find the username corresponding to uid
 * 
 * @param uid 
 * @return user name
 */
char *uidToName(uid_t uid)
{
    struct passwd *pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(numstr, "%d", uid);
        return numstr;
    }
    else
        return pw_ptr->pw_name;
}

/**
 * @brief Find the groupname corresponding to gid
 * 
 * @param gid 
 * @return groupname
 */
char *gidToName(gid_t gid)
{
    struct group *grp_ptr;
    static char numstr[10];
    if ((grp_ptr = getgrgid(gid)) == NULL)
    {
        sprintf(numstr, "%d", gid);
        return numstr;
    }
    else
        return grp_ptr->gr_name;
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

    showDirecoryList(path); //开始文件夹递归打印
    return 0;
}