#include <iostream>
#include <fcntl.h>
#include <vector>
#include <unistd.h>

using namespace std;

class CA_LL
{
private:
    /* data */
    int i;

public:
    CA_LL(/* args */);
    ~CA_LL();

    void f(); //接口告知

    //int member;
    bool Serialize(const char *pFilePath);
    bool Deserialize(const char *pFilePath);
};

CA_LL::CA_LL(/* args */)
{
    i = 0;
}

CA_LL::~CA_LL()
{
}

void CA_LL::f(void)
{
    std::cout << "set i:" << i << std::endl;
}

bool CA_LL::Serialize(const char *pFilePath)
{

    int fd = open(pFilePath, O_RDWR | O_CREAT | O_TRUNC); //输出到文件
    
    if (fd == -1) //检查打开是否正确
    {
        std::cout << "fd == -1" << std::endl;
        return false;
    }
    if (write(fd, &i, sizeof(int)) == -1) //写入检查
    {
        close(fd);
        return false;
    }
    if (close(fd) == -1) // 文件关闭检查
    {
        std::cout << "close error" << std::endl;
        return false;
    }
    std::cout << "finish!" << std::endl;
    return true;
}

bool CA_LL::Deserialize(const char *pFilePath)
{
    int fd = open(pFilePath, O_RDWR); //输出到文件
    if (fd == -1)
    {
        return false;
    }
    if (read(fd, &i, sizeof(int)) == -1)
    {
        close(fd);
        return false;
    }
    if (close(fd) == -1)
    {
        return false;
    }
    return true;
}

int main()
{
    CA_LL cal;
    int flag1;
    cal.f();
    flag1 = cal.Serialize("data");
    std::cout << "serialize return:" << flag1 << std::endl;
    //TODO 新建文件无法正常关闭，导致写入空白
    return 0;
}