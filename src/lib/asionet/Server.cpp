#include "Server.h"


void Server::OnData(uint8_t *pData, uint32_t len)
{
    Send(pData, len);
}

Server* CreateServer(SocketPtr spSock)
{
    return new Server(spSock);
}