#ifndef PLUGINENUMERATOR_H
#define PLUGINENUMERATOR_H

#include <vector>
#include <string>

using namespace std;

class PluginEnumerator
{
public:
    PluginEnumerator(/* args */);
    ~PluginEnumerator();

    bool GetPluginNames(vector<string> &vStrPlugin);
};

#endif