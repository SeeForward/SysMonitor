#include "Server.h"


void Server::OnData(uint8_t *pData, uint32_t len)
{
    uint8_t *pSend = new uint8_t[len];
    memcpy(pSend, pData, len);
    Send(pSend, len);
}

Server* CreateServer(SocketPtr spSock)
{
    return new Server(spSock);
}