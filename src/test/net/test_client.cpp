
#include <iostream>
#include <string.h>
#include "ClientSocket.h"

using namespace std;

int main(int argc, const char * argv[])
{
    ClientSocket client;

    SocketAddress serverAddr("127.0.0.1", 19999);

    int rc = client.Connect(serverAddr);
    if (0 != rc)
    {
        cout << "connect error" << endl;
        return -1;
    }

    char buf[100] = "client message";
    rc = client.Send((uint8_t*)buf, strlen(buf));
    if (strlen(buf) != rc)
    {
        cout << "send error" << endl;
        return -2;
    }

    rc = client.Recv((uint8_t*)buf, 100);
    if (rc <= 0)
    {
        cout << "recv error" << endl;
        return -3;
    }

    buf[rc] = '\0';
    cout << buf << endl;

    return 0;
}
