#include <iostream>
#include <fcntl.h>
#include <vector>
#include <unistd.h>

using namespace std;

/**
 * @brief 单一成员序列化类 
 * 针对给定文件位置，序列化和反序列化成员变量 member 
 */
class SerializeSingle
{
private:
    int member; //成员变量

public:
    SerializeSingle(void);
    SerializeSingle(int num);
    ~SerializeSingle();

    void showMember(void);      //显示成员变量内容
    bool serialize(FILE *fp);   //单个成员变量的序列化
    bool deserialize(FILE *fp); //单个成员变量的反序列化
};

SerializeSingle::SerializeSingle(void)
{
    member = 0;
}
//赋值初始化
SerializeSingle::SerializeSingle(int num)
{
    member = num;
}

SerializeSingle::~SerializeSingle()
{
}

void SerializeSingle::showMember(void)
{
    std::cout << "member:" << member << ' ' << std::flush;
}

/**
 * @brief 单变量序列化函数
 * @param fp    FILE结构体指针
 * 
 * @return 是否完成
 */
bool SerializeSingle::serialize(FILE *fp)
{
    if (fwrite(&member, sizeof(member), 1, fp) <= 0) //写入序列化i，并判断写是否成功
    {
        std::cout << "write error!" << std::endl;
        return false;
    }
    return true;
}
/**
 * @brief 单一结构体的反序列化
 * @param fp    FILE结构体指针
 * 
 * @return 是否完成
 */
bool SerializeSingle::deserialize(FILE *fp)
{
    if (fread(&member, sizeof(member), 1, fp) <= 0) //写入序列化i，并判断写是否成功
    {
        std::cout << "read error!" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief 多个SerializeSingle类的成员变量序列化类
 * 针对给定文件位置，序列化或反向序列化类数组的成员变量
 */
class multi_memberSerialize
{
public:
    multi_memberSerialize(void);
    ~multi_memberSerialize();

    bool multiSerialize(const char *pFilePath, std::vector<SerializeSingle> &clMembers);
    bool demultiSerialize(const char *pFilePath, std::vector<SerializeSingle> &clMembers);
};

multi_memberSerialize::multi_memberSerialize(void)
{
}

multi_memberSerialize::~multi_memberSerialize()
{
}

/**
 * @brief 多个类的序列化
 * @param pFilePath 需要保持的文件名及路径
 * @param clMembers 实例化SerializeSingle类的数组
 * 
 * @return 是否完成
 */
bool multi_memberSerialize::multiSerialize(const char *pFilePath, std::vector<SerializeSingle> &clMembers)
{
    int length = clMembers.size(); //获取数组大小
    FILE *fp;                      //文件指针
    fp = fopen(pFilePath, "a");    //追加打开文件
    if (fp == NULL)
    {
        std::cout << "open error!" << std::endl;
        return false;
    }
    for (int i = 0; i < length; ++i)
    {
        //对类数组的成员变量逐一调用序列化并检查
        if (clMembers[i].serialize(fp) == false)
        {
            std::cout << "No." << i << ' ' << std::flush; //报告出错位置
            return false;
        }
    }
    if (fclose(fp) != 0) //文件关闭检查
    {
        std::cout << "close error!" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief 多个类的反序列化
 * @param pFilePath 需要保持的文件名及路径
 * @param clMembers 用于存储的SerializeSingle类的数组
 * 
 * @return 是否完成
 */
bool multi_memberSerialize::demultiSerialize(const char *pFilePath, std::vector<SerializeSingle> &clMembers)
{
    FILE *fp;                   //文件指针
    fp = fopen(pFilePath, "r"); //只读打开文件
    if (fp == NULL)
    {
        std::cout << "open error!" << std::endl;
        return false;
    }
    //循环读取
    for (int i = 0;; i++)
    {
        SerializeSingle clSer; //存储读取出来的变量
        bool flag;             //反编译完成
        //对类数组的成员变量逐一调用反序列化并检查
        flag = clSer.deserialize(fp);
        if (flag == false)
        {
            std::cout << "total:" << i << " " << std::flush; //报告返回位置
            break;
        }
        else
        {
            clMembers.push_back(clSer);
        }
    }
    if (fclose(fp) != 0) //文件关闭检查
    {
        std::cout << "close error!" << std::endl;
        return false;
    }
    return true;
}

int main(void)
{
    int flag1; //完成标识
    {
        //实例化类，并赋予member初始化
        SerializeSingle s1(10), s2(1), s3(6);
        //用数组存放这些类
        std::vector<SerializeSingle> clSer;
        clSer.push_back(s1);
        clSer.push_back(s2);
        clSer.push_back(s3);
        //将这些类的member序列化到文件data中
        multi_memberSerialize mulSer;
        flag1 = mulSer.multiSerialize("data", clSer);
        std::cout << "serialize return:" << (flag1 ? "succeed" : "fail") << std::endl;
    }
    {
        //实例化多成员序列化类
        multi_memberSerialize mulSer;
        //用数组存放这些类
        std::vector<SerializeSingle> clSer;
        flag1 = mulSer.demultiSerialize("data", clSer);
        if (flag1 == false) //反编译失败
        {
            std::cout << "serialize return:fail" << std::endl;
            return 0;
        }
        int length = clSer.size(); //获取反序列变量长度，防止越界
        for (int i = 0; i < length; i++)
        {
            //将反序列内容打印
            std::cout << "No." << i << '_' << std::flush;
            clSer[i].showMember();
            printf("\n");
        }
    }
    return 0;
}