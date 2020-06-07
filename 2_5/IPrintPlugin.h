
/**
 * @brief 这是动态链接库的接口类
 * 
 */
class IPrintPlugin
{
public:
    IPrintPlugin(/* args */);
    virtual ~IPrintPlugin();

    virtual void Help() = 0;
    virtual void PrintStr() = 0;
    virtual int GetID() = 0;
};
