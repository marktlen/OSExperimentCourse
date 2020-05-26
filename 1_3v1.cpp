#include <iostream>
#include <fcntl.h>
#include <vector>
#include <unistd.h>

using namespace std;

//---------------
// Type of Class
//---------------
#define TYPE_A 0
#define TYPE_B 1

/**
 * @brief Object serialization structure, identifies type by mark and corresponding content
 */
struct Serialized
{
    int nType;  /* Mark for Class, mark A whit 0, mark B whit 1 */
    void *pObj; /* Pointer to Class member */
};

/**
 * @brief Type A class
 */
class TypeAs
{
private:
    int member;

public:
    TypeAs();
    TypeAs(int num);
    ~TypeAs();

    void showMember(void);      //show the value of member
    bool serialize(FILE *fp);   //serialize members
    bool deserialize(FILE *fp); //deserialize members
};

TypeAs::TypeAs()
{
    member = 0;
}
TypeAs::TypeAs(int num)
{
    member = num;
}

TypeAs::~TypeAs()
{
}

void TypeAs::showMember(void)
{
    std::cout << "member:" << member << ' ' << std::flush;
}

/**
 * @brief serialize members
 * @param fp    printer of FILE sturct
 * 
 * @return finish is true, fail is false
 */
bool TypeAs::serialize(FILE *fp)
{
    Serialized ser;
    if (fwrite(&member, sizeof(member), 1, fp) <= 0) //write member into file
    {
        std::cout << "write error!" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief deserialize members
 * @param fp    printer to FILE
 * 
 * @return finish is true, fail is false
 */
bool TypeAs::deserialize(FILE *fp)
{
    if (fread(&member, sizeof(member), 1, fp) <= 0) //read member from file
    {
        std::cout << "read error!" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief Type B class
 */
class TypeBs
{
private:
    int member_x;
    int member_y;

public:
    TypeBs();
    TypeBs(int x, int y);
    ~TypeBs();

    void showMember(void);      //show the value of member
    bool serialize(FILE *fp);   //serialize members
    bool deserialize(FILE *fp); //deserialize members
};

TypeBs::TypeBs()
{
    member_x = 0;
    member_y = 0;
}
TypeBs::TypeBs(int x, int y)
{
    member_x = x;
    member_y = y;
}

TypeBs::~TypeBs()
{
}

void TypeBs::showMember(void)
{
    std::cout << "member:" << member_x << ' ' << member_y << std::flush;
}

/**
 * @brief serialize members
 * @param fp    printer of FILE sturct
 * 
 * @return finish is true, fail is false
 */
bool TypeBs::serialize(FILE *fp)
{
    if (fwrite(&member_x, sizeof(member_x), 1, fp) <= 0) //write member_x into file
    {
        std::cout << "write x error!" << std::endl;
        return false;
    }
    if (fwrite(&member_y, sizeof(member_y), 1, fp) <= 0) //write member_y into file
    {
        std::cout << "write y error!" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief deserialize members
 * @param fp    printer to FILE
 * 
 * @return finish is true, fail is false
 */
bool TypeBs::deserialize(FILE *fp)
{
    //read member from file
    if (fread(&member_x, sizeof(member_x), 1, fp) <= 0)
    {
        std::cout << "read x error!" << std::endl;
        return false;
    }
    if (fread(&member_y, sizeof(member_y), 1, fp) <= 0)
    {
        std::cout << "read y error!" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief multi-member serialize
 */
class multi_memberSerialize
{
public:
    multi_memberSerialize(void);
    ~multi_memberSerialize();

    bool multiSerialize(const char *pFilePath, std::vector<Serialized> &sereds);
    bool demultiSerialize(const char *pFilePath, std::vector<Serialized> &sereds);
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
 * @param clMembers 传入需要序列化的序列化数据结构体的数组
 * 
 * @return 是否完成
 */
bool multi_memberSerialize::multiSerialize(const char *pFilePath, std::vector<Serialized> &sereds)
{
    int length = sereds.size(); //获取数组大小
    FILE *fp;                   //文件指针
    fp = fopen(pFilePath, "a"); //追加打开文件
    if (fp == NULL)
    {
        std::cout << "open error!" << std::endl;
        return false;
    }
    for (int i = 0; i < length; ++i)
    {
        if (fwrite(&sereds[i].nType, sizeof(int), 1, fp) <= 0)
        {
            fclose(fp);
            std::cout << "write error!" << std::endl;
            return false;
        }
        if (sereds[i].nType == TYPE_A)
        {
            TypeAs *pa = (TypeAs *)(sereds[i].pObj); //将传入的数据赋给类

            if (pa->serialize(fp) == false)
            {
                std::cout << "A serialize error!" << std::endl;
                return false;
            }
        }
        else if (sereds[i].nType == TYPE_B)
        {
            TypeBs *pb = (TypeBs *)(sereds[i].pObj);

            if (pb->serialize(fp) == false)
            {
                std::cout << "B serialize error!" << std::endl;
                return false;
            }
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
 * @param clMembers 用于存储的反序列化产生数据结构体的数组
 * 
 * @return 是否完成
 */
bool multi_memberSerialize::demultiSerialize(const char *pFilePath, std::vector<Serialized> &sereds)
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
        int nType;
        if (fread(&nType, sizeof(int), 1, fp) <= 0)
        {
            //std::cout << "total:" << i << " " << std::flush; //报告返回位置
            break;
        }

        if (nType == TYPE_A) //对不同类型做不同处理
        {
            TypeAs *pa;          //声明A类指针
            pa = new TypeAs;     //将指针指向一个实例化的A类
            pa->deserialize(fp); //按A类反序列化内容

            Serialized ser; //将结果装入结构体中
            ser.nType = nType;
            ser.pObj = pa;

            sereds.push_back(ser); //装入结构体数组中
        }
        else if (nType == TYPE_B)
        {
            TypeBs *pb;          //声明B类指针
            pb = new TypeBs;     //将指针指向一个实例化的B类
            pb->deserialize(fp); //按B类反序列化内容

            Serialized ser;
            ser.nType = nType;
            ser.pObj = pb;

            sereds.push_back(ser);
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
        //实例化两个A，两个B
        TypeAs a1(1);
        Serialized s1;
        s1.nType = TYPE_A;
        s1.pObj = &a1;

        TypeBs b1(2, 3);
        Serialized s2;
        s2.nType = TYPE_B;
        s2.pObj = &b1;
        TypeBs b2(4, 5);
        Serialized s3;
        s3.nType = TYPE_B;
        s3.pObj = &b2;

        TypeAs a2(6);
        Serialized s4;
        s4.nType = TYPE_A;
        s4.pObj = &a2;

        //放入数组中
        std::vector<Serialized> ser;
        ser.push_back(s1);
        ser.push_back(s2);
        ser.push_back(s3);
        ser.push_back(s4);

        //将数组中的内容序列化到文件
        multi_memberSerialize multiser;
        flag1 = multiser.multiSerialize("data", ser);
        std::cout << "serialize return:" << (flag1 ? "succeed" : "fail") << std::endl;
    }
    {
        //声明反序列化方法类，存放数组和目标文件
        multi_memberSerialize multiser;
        std::vector<Serialized> ser;
        flag1 = multiser.demultiSerialize("data", ser);
        std::cout << "deserialize return:" << (flag1 ? "succeed" : "fail") << std::endl;

        int length = ser.size(); //获取反序列数组长度
        std::cout << "total:" << length << std::endl;
        for (int i = 0; i < length; i++)
        {
            //判断类型，并将数据存入类中
            if (ser[i].nType == TYPE_A)
            {
                TypeAs *pa = (TypeAs *)(ser[i].pObj);
                pa->showMember();
            }
            else if (ser[i].nType == TYPE_B)
            {
                TypeBs *pb = (TypeBs *)(ser[i].pObj);
                pb->showMember();
            }
            printf("\n");
        }
    }
    return 0;
}