#include "Server.h"

Server* CreateServer(SocketPtr spSock)
{
    return new Server(spSock);
}