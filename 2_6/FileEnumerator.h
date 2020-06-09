#include <vector>
#include <string>

class FileEnumerator
{
public:
    FileEnumerator(/* args */);
    ~FileEnumerator();

    int FileEnum(char *path); //枚举目录下文件
    void showFileList();      //打印读取到的文件路径
    std::vector<std::string> vfilePaths;
};