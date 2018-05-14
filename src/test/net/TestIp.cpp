
#include <iostream>
#include "IPAddress.h"

using namespace std;

void test_ip(const string &ipStr)
{
    IPAddress ip(ipStr);

    cout << "IP:" << ip.Str() << ",";

    cout << "Valid:" << ip.Valid() << endl;
}


int main(int argc, const char * argv[])
{
    test_ip("127.0.0.1");
    test_ip("127.0.0.1 ");
    test_ip(" 127.0.0.1 ");
    test_ip(" 127.0.0.1");
    test_ip(" 127.0.0. 1");
    test_ip("-127.0.0.1");
    test_ip("256.0.0.1");
    return 0;
}
