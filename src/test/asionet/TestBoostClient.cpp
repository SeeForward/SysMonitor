#include "Client.h"
#include "SystemTool.h"
#include <iostream>
#include <boost/bind.hpp>
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>

IOServiceType iosev;

void ServiceRun()
{
    iosev.run();
}

using namespace std;

int main(int argc, const char* argv[])
{
    Client client(iosev);

    int rc = client.Connect("127.0.0.1", 12345);

    if (rc != 0)
    {
        cout << rc << endl;
        cout << "client connect fail" << endl;
        return rc;
    }


    char buf[512] = {0};

    std::thread thd(ServiceRun);
    thd.detach();

    int i = 0;
    while (true)
    {
        cin.getline(buf, 512);
        //cin >> buf;
        std::string str(buf);
        cout << buf << endl;
        NetMessagePtr pMsg = new NetMessage(i, str);
        string str2;
        pMsg->ToStr(str2);
        uint32_t size = str2.size();
        uint8_t *pData = new uint8_t[size + sizeof(uint32_t)];
        memcpy(pData, &size, sizeof(uint32_t));
        memcpy(pData + sizeof(uint32_t), str2.c_str(), size); 
        client.Send(pData, size + sizeof(uint32_t));
        ++i;
    }
    client.Close();

    return 0;
}
