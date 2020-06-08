#include <vector>
#include <string>

class FileEnumerator
{
public:
    FileEnumerator(/* args */);
    ~FileEnumerator();

    int FileEnum(char *path);
    void showFileList();
    std::vector<std::string> vfilePaths;
};