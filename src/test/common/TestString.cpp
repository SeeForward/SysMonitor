#include <iostream>
#include "StringUtil.h"

#include <vector>
#include <string>

using namespace std;

void test_string_split()
{
    string ip = "127.0.0.1";

    vector<string> strs;
    StringUtil::Split(ip, '.', strs);

    for (size_t i = 0; i < strs.size(); ++i)
    {
        cout << strs[i] << endl;
    }
}


int main(int argc, const char * argv[])
{
    test_string_split();
    return 0;
}