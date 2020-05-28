#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

int main()
{
    //这是随便写的，方便查看文件修改的位置
    char const *str = "1234567890abc";
    char *readFileStr;
    // 读写打开，没有就创建
    int fd = open("work1Doc.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (-1 == fd) //打开错误判断
    {
        cout << "open error" << endl;
        return 0;
    }
    if (write(fd, str, strlen(str)) <= 0) //写入错误判断
    {
        cout << "open error" << endl;
        return 0;
    }

    system("cat work1Doc.txt");                                //读取打印当前字符串
    off_t CurrentPosition = lseek(fd, 0, SEEK_CUR);            //获取当前偏移量
    cout << "\tCurrentPosition:  " << CurrentPosition << endl; //当前文件偏移量 13

    char buf = 'm';
    if (write(fd, &buf, 1) == -1) //写入一个字符
    {
        cout << "write error" << endl;
        close(fd);
        return 0;
    }

    system("cat work1Doc.txt");                                                  //读取打印当前字符串
    CurrentPosition = lseek(fd, 0, SEEK_CUR);                                    //获取当前偏移量
    cout << "\tAfter writing, 'm' is add after file,\n\t\tCurrentPosition:  " 
    << CurrentPosition << endl; //修改后文件尾部的偏移量 14

    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        cout << "lseek error" << endl;
        close(fd);
        return 0;
    }
    system("cat work1Doc.txt"); //读取打印当前字符串
    off_t NewCurrentPosition = lseek(fd, 0, SEEK_CUR);
    cout << "\tNewCurrentPosition:  " << NewCurrentPosition << endl; //文件还可以把偏移量修改到头部

    char buf1 = 'P'; //如果修改成功，查看p的出现位置
    if (write(fd, &buf1, 1) == -1)
    {
        cout << "write error" << endl;
        close(fd);
        return 0;
    }
    system("cat work1Doc.txt"); //读取打印当前字符串
    cout << "\t'P' is replaced the first letter"<<endl;
    close(fd);
    return 0;
}