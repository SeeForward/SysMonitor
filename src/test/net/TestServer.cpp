
#include <iostream>
#include <string.h>
#include "ServerSocket.h"
#include "SystemTool.h"

using namespace std;

int main(int argc, const char * argv[])
{
    SocketAddress listenAddr("127.0.0.1", 66668);

    ServerSocket server(listenAddr);
    int rc = server.Listen(5);
    if (rc < 0)
    {
        cout << "listen error : " << errno << endl;
        return -errno;
    }

    ClientSocket client;

    rc = server.Accept(client);
    if (rc < 0)
    {
        cout << "accept error : " << errno << endl;
        return -errno;
    }

    cout << "local: " << client.LocalAdderss().ToStr() << endl;
    cout << "remote: " << client.RemoteAdderss().ToStr() << endl;

    char buf[100] = "";
    rc = client.Recv((uint8_t*)buf, 100);
    if (rc <= 0)
    {
        cout << "recv error : " << errno << endl;
        return -errno;
    }

    buf[rc] = '\0';
    cout << buf << endl;

    char buf2[100] = "server message";
    rc = client.Send((uint8_t*)buf2, strlen(buf2));
    if (strlen(buf2) != rc)
    {
        cout << "send error : " << errno << endl;
        return -errno;
    }

    SleepMs(1000 * 1000);


    return 0;
}
