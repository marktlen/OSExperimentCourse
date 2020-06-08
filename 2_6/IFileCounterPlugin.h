
/**
 * @brief 这是文件统计插件的接口类
 * 
 */
class IFileCounterPlugin
{
public:
    IFileCounterPlugin(/* args */);
    virtual ~IFileCounterPlugin();

    virtual void Help() = 0;
    virtual bool FileInf(const char *path) = 0;
    virtual int GetID() = 0;
};
