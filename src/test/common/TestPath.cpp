
#include "Path.h"

#include <iostream>


using namespace std;

int main(int argc, const char *argv[])
{
    Path p = Path::GetExecuteFilePath();

    cout << "path\t:" << p.Str() << endl;
    string dir = p.ExtractPath();
    cout << "dir\t:" << dir << endl;
    string name = p.ExtractFileName();
    cout << "name\t:" << name << endl;
    cout << "exist\t:" << p.IsExist() << endl;
    cout << "abs\t:" << p.IsAbsolute() << endl;
    cout << "dir\t:" << p.IsDirectory() << endl;
    cout << "file\t:" << p.IsFile() << endl;

    cout << "--------------------------------------------------" << endl;

    Path dirPath(dir);
    cout << "path\t:" << dirPath.Str() << endl;
    cout << "dir\t:" << dirPath.ExtractPath() << endl;
    cout << "name\t:" << dirPath.ExtractFileName() << endl;
    cout << "exist\t:" << dirPath.IsExist() << endl;
    cout << "abs\t:" << dirPath.IsAbsolute() << endl;
    cout << "dir\t:" << dirPath.IsDirectory() << endl;
    cout << "file\t:" << dirPath.IsFile() << endl;

    return 0;
}
