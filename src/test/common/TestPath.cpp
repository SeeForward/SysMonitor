
#include "Path.h"

#include <iostream>


using namespace std;

int main(int argc, const char *argv[])
{
    Path p;
    p.SetPath(Path::GetExecuteFilePath());

    cout << p.Str() << endl;

    cout << p.ExtractPath() << endl;

    cout << p.ExtractFileName() << endl;

    cout << "Exist:" << p.IsExist() << endl;

    cout << "Absolute:" << p.IsAbsolute() << endl;

    cout << "Directory:" << p.IsDirectory() << endl;

    return 0;
}
