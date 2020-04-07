
#include <iostream>
#include <string.h>
#include "ClientSocket.h"
#include "SystemTool.h"

using namespace std;

int main(int argc, const char * argv[])
{
    ClientSocket client;

    SocketAddress serverAddr("127.0.0.1", 66668);

    int rc = client.Connect(serverAddr);
    if (0 != rc)
    {
        cout << "connect error, rc : " << rc << endl;
        return -rc;
    }

    cout << "local: " << client.LocalAdderss().ToStr() << endl;
    cout << "remote: " << client.RemoteAdderss().ToStr() << endl;

    char buf[100] = "client message";
    rc = client.Send((uint8_t*)buf, strlen(buf));
    if (strlen(buf) != rc)
    {
        cout << "send error, rc : " << rc << endl;
        return -rc;
    }

    rc = client.Recv((uint8_t*)buf, 100);
    if (rc <= 0)
    {
        cout << "recv error, rc : " << rc << endl;
        return -rc;
    }

    buf[rc] = '\0';
    cout << buf << endl;

    SleepMs(1000 * 1000);

    return 0;
}
