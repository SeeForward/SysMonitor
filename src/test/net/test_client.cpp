
#include <iostream>
#include <string.h>
#include "ClientSocket.h"
#include "system_tool.h"

using namespace std;

int main(int argc, const char * argv[])
{
    ClientSocket client;

    SocketAddress serverAddr("127.0.0.1", 66668);

    int rc = client.Connect(serverAddr);
    if (0 != rc)
    {
        cout << "connect error, rc : " << errno << endl;
        return -errno;
    }

    cout << "local: " << client.LocalAdderss().ToStr() << endl;
    cout << "remote: " << client.RemoteAdderss().ToStr() << endl;

    char buf[100] = "client message";
    rc = client.Send((uint8_t*)buf, strlen(buf));
    if (strlen(buf) != rc)
    {
        cout << "send error, rc : " << errno << endl;
        return -errno;
    }

    rc = client.Recv((uint8_t*)buf, 100);
    if (rc <= 0)
    {
        cout << "recv error, rc : " << errno << endl;
        return -errno;
    }

    buf[rc] = '\0';
    cout << buf << endl;

    SleepMs(1000 * 1000);

    return 0;
}
