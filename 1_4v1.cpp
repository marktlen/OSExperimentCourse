#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;

//---------------
// Type of Class
//---------------
#define TYPE_A 0
#define TYPE_B 1

/**
 * @brief 作为借口类，方便类型添加
 * 使用纯虚函数Type类为虚基类，不能单独进行实例化
 */
class Type
{
public:
    Type(/* args */){};
    virtual ~Type(){};

    virtual bool Serialize(FILE *fp) = 0;
    virtual Type *Deserialize(FILE *fp) = 0; // 反序列化内容返回由子类决定
    virtual bool GetType(int &type) = 0;
};

/**
 * @brief Type A class
 */
class TypeAs : public Type
{
private:
    int member;

public:
    TypeAs();
    explicit TypeAs(int num);
    virtual ~TypeAs();

    void showMember(void);            //show the value of member
    virtual bool Serialize(FILE *fp); //serialize members
    virtual bool GetType(int &type);  //get type ID

    /**
     * @brief deserialize members
     * @param fp    printer to FILE
     * 
     * @return deserialize data with class of Type
     */
    virtual Type *Deserialize(FILE *fp) //deserialize members
    {
        TypeAs *pa = new TypeAs();

        if (fread(&(pa->member), sizeof(pa->member), 1, fp) <= 0) //read member from file
        {
            std::cout << "read error!" << std::endl;
            delete pa;
            return NULL;
        }
        return pa; //将member返回成Type类
    }
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

bool TypeAs::GetType(int &type)
{
    type = TYPE_A;
    return true;
}

/**
 * @brief serialize members
 * @param fp    printer of FILE sturct
 * 
 * @return finish is true, fail is false
 */
bool TypeAs::Serialize(FILE *fp)
{
    if (fp == NULL)
    {
        return false;
    }

    if (fwrite(&member, sizeof(member), 1, fp) <= 0) //write member into file
    {
        std::cout << "write error!" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief Type B class
 */
class TypeBs : public Type
{
private:
    int member_x;
    int member_y;

public:
    TypeBs();
    explicit TypeBs(int x, int y);
    virtual ~TypeBs();

    void showMember(void);            //show the value of member
    virtual bool Serialize(FILE *fp); //serialize members
    virtual bool GetType(int &type);  // get type ID

    /**
     * @brief deserialize members
     * @param fp    printer to FILE
     * 
     * @return deserialize data with class of Type
     */
    virtual Type *Deserialize(FILE *fp)
    {
        TypeBs *pb = new TypeBs();

        if (fread(&(pb->member_x), sizeof(member_x), 1, fp) <= 0)
        {
            std::cout << "read x error!" << std::endl;
            delete pb;
            return NULL;
        }
        if (fread(&(pb->member_y), sizeof(member_y), 1, fp) <= 0)
        {
            std::cout << "read y error!" << std::endl;
            delete pb;
            return NULL;
        }
        return pb; //将获取的B类x，y返回
    }
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

bool TypeBs::GetType(int &type)
{
    type = TYPE_B;
    return true;
}

void TypeBs::showMember(void)
{
    std::cout << "member:(" << member_x << ',' << member_y << ')' << std::flush;
}

bool GetType(int &type)
{
    type = TYPE_B;
    return true;
}

/**
 * @brief serialize members
 * @param fp    printer of FILE sturct
 * 
 * @return finish is true, fail is false
 */
bool TypeBs::Serialize(FILE *fp)
{
    if (fp == NULL)
    {
        return false;
    }
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
 * @brief multi-member serialize
 */
class multi_memberSerialize
{
private:
    std::vector<Type *> m_Types; //已有继承类型管理，作为注册队列
public:
    multi_memberSerialize(void);
    ~multi_memberSerialize();

    bool multiSerialize(const char *pFilePath, std::vector<Type *> &ser);
    bool demultiSerialize(const char *pFilePath, std::vector<Type *> &ser);
    void RegisterType(Type *ptype); //已有类型注册函数
};

multi_memberSerialize::multi_memberSerialize(void)
{
}

multi_memberSerialize::~multi_memberSerialize()
{
}

void multi_memberSerialize::RegisterType(Type *ptype)
{
    m_Types.push_back(ptype);
}

/**
 * @brief 多个类的序列化
 * @param pFilePath 需要保持的文件名及路径
 * @param clMembers 传入需要序列化的序列化数据结构类的数组
 * 
 * @return 是否完成
 */
bool multi_memberSerialize::multiSerialize(const char *pFilePath, std::vector<Type *> &ser)
{
    int length = ser.size();    //获取数组大小
    FILE *fp;                   //文件指针
    fp = fopen(pFilePath, "a"); //追加打开文件
    if (fp == NULL)
    {
        std::cout << "open error!" << std::endl;
        return false;
    }
    for (int i = 0; i < length; ++i)
    {
        // Fill in the serial number and data in order
        int type;
        ser[i]->GetType(type); // get type
        if (fwrite(&type, sizeof(int), 1, fp) <= 0)
        {
            fclose(fp);
            std::cout << "read error!" << std::endl;
            return false;
        }
        ser[i]->Serialize(fp);
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
 * @param clMembers 用于存储的反序列化产生数据结构类的数组
 * 
 * @return 是否完成
 */
bool multi_memberSerialize::demultiSerialize(const char *pFilePath, std::vector<Type *> &ser)
{
    FILE *fp;                   //文件指针
    fp = fopen(pFilePath, "r"); //只读打开文件
    if (fp == NULL)
    {
        std::cout << "open error!" << std::endl;
        return false;
    }

    int countSize = m_Types.size();
    //循环读取
    while (true)
    {
        int nType;
        if (fread(&nType, sizeof(int), 1, fp) <= 0) //获取文件中数据类型ID
        {
            //std::cout << "total:" << i << " " << std::flush; //报告返回位置
            break;
        }

        int currentType; //遍历的当前类型
        for (int i = 0; i < countSize; i++)
        {
            m_Types[i]->GetType(currentType); //获取当前类型
            // 当类型ID属于当前类型时，按当前类型对数据进行反序列化操作
            if (nType == currentType)
            {
                Type *ptype = m_Types[i]->Deserialize(fp);
                // 如何获取序列化内容，存入传入数据中
                if (ptype != NULL)
                {
                    ser.push_back(ptype);
                }
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

int main(void)
{
    int flag1; //完成标识
    {
        //实例化两个A，两个B
        TypeAs a1(1);
        TypeBs b1(2, 3);
        TypeBs b2(4, 5);
        TypeAs a2(6);

        //放入数组中
        std::vector<Type *> ser;
        ser.push_back(&a1);
        ser.push_back(&b1);
        ser.push_back(&b2);
        ser.push_back(&a2);

        //将数组中的内容序列化到文件
        multi_memberSerialize multiser;
        flag1 = multiser.multiSerialize("data", ser);
        std::cout << "serialize return:" << (flag1 ? "succeed" : "fail") << std::endl;
    }
    {
        //声明反序列化方法类，存放数组和目标文件
        multi_memberSerialize multiser;

        //注册A和B两种类型
        TypeAs a;
        TypeBs b;
        multiser.RegisterType(&a);
        multiser.RegisterType(&b);

        //用于存放反序列化内容
        std::vector<Type *> ser;
        flag1 = multiser.demultiSerialize("data", ser);
        std::cout << "deserialize return:" << (flag1 ? "succeed" : "fail") << std::endl;

        int length = ser.size(); //获取反序列数组长度
        std::cout << "total:" << length << std::endl;
        for (int i = 0; i < length; i++)
        {
            //对获取数据进行类型转换，如果类型转换正确就打印出来
            TypeAs *pa = dynamic_cast<TypeAs *>(ser[i]);
            if (pa != NULL)
            {
                pa->showMember();
                printf("\n");
                continue;
            }
            TypeBs *pb = dynamic_cast<TypeBs *>(ser[i]);
            if (pb != NULL)
            {
                pb->showMember();
                printf("\n");
            }
        }
    }
    return 0;
}