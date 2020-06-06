#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;

/**
 * @brief 单个对象的实例化
 * 
 */
class Serialize_single
{
private:
    /* data */
    int member;

public:
    Serialize_single();
    ~Serialize_single();

    void show_member(); //打印内容

    //int member;
    bool Serialize(const char *pFilePath);
    bool Deserialize(const char *pFilePath);
};

Serialize_single::Serialize_single()
{
    member = 1016; // last four digits of my student number
}

Serialize_single::~Serialize_single()
{
}

void Serialize_single::show_member(void)
{
    std::cout << "member:" << member << std::endl;
}

/**
 * @brief 序列化到文件中
 * 
 * @param pFilePath 文件路径
 * @return true 成功
 * @return false 错误
 */
bool Serialize_single::Serialize(const char *pFilePath)
{
    FILE *fp;                                  //文件指针
    if ((fp = fopen(pFilePath, "w+")) == NULL) //读写打开文件
    {
        std::cout << "open error!" << std::endl;
        return false;
    }
    if (fwrite(&member, sizeof(member), 1, fp) <= 0) //写入序列化i，并判断写是否成功
    {
        std::cout << "write error!" << std::endl;
        return false;
    }
    if (fclose(fp) == EOF) //文件关闭检查
    {
        std::cout << "close error!" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief 从文件中反序列化
 * 
 * @param pFilePath 文件路径
 * @return true 成功
 * @return false 失败
 */
bool Serialize_single::Deserialize(const char *pFilePath)
{
    FILE *fp;                                 //文件指针
    if ((fp = fopen(pFilePath, "r")) == NULL) //只读打开文件
    {
        std::cout << "open error!" << std::endl;
        return false;
    }
    if (fread(&member, sizeof(member), 1, fp) <= 0) //写入序列化i，并判断写是否成功
    {
        std::cout << "read error!" << std::endl;
        return false;
    }
    if (fclose(fp) == EOF) //文件关闭检查
    {
        std::cout << "close error!" << std::endl;
        return false;
    }
    return true;
}

int main()
{
    int flag1; //完成标识
    {
        Serialize_single cal;
        cal.show_member();
        flag1 = cal.Serialize("data");
        std::cout << "serialize return:" << (flag1 ? "true" : "flase") << std::endl;
    }
    {
        Serialize_single cal;
        flag1 = cal.Deserialize("data");
        cal.show_member();
        std::cout << "Deserialize return:" << (flag1 ? "true" : "flase") << std::endl;
    }
    return 0;
}