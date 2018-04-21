
#include <iostream>
#include <string.h>
#include "ServerSocket.h"

using namespace std;

int main(int argc, const char * argv[])
{
    ServerSocket server;
    SocketAddress serverAddr("127.0.0.1", 19999);

    server.SetLocalAddress(serverAddr);
    server.Listen(5);

    ClientSocket client;

    server.Accept(client);

    char buf[100] = "";
    int rc = client.Recv((uint8_t*)buf, 100);
    if (rc <= 0)
    {
        cout << "recv error" << endl;
        return -3;
    }

    buf[rc] = '\0';
    cout << buf << endl;

    char buf2[100] = "server message";
    rc = client.Send((uint8_t*)buf2, strlen(buf2));
    if (strlen(buf2) != rc)
    {
        cout << "send error" << endl;
        return -2;
    }

    return 0;
}
