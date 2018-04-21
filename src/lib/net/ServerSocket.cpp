#include "ServerSocket.h"

#ifdef __WINDOWS__
#include<winsock2.h>
#else //__LINUX__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

ServerSocket::ServerSocket()
{
    m_sock = socket(AF_INET,SOCK_STREAM,0);
}

ServerSocket::ServerSocket(const SocketAddress &local) 
{
    m_local = local;
    m_sock = socket(AF_INET,SOCK_STREAM,0);
}

ServerSocket::~ServerSocket() {}

int ServerSocket::Listen(int maxNum)
{
    sockaddr_in serverAddr;
    if (m_local.IP().Valid())
    {
        serverAddr.sin_addr.s_addr = inet_addr(m_local.IP().Str().c_str());
    }
    else
    {
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(m_local.Port());
    int rc = bind(m_sock, (sockaddr*)&serverAddr, sizeof(sockaddr));
    if (0 != rc)
    {
        return rc;
    }
    
    return listen(m_sock, maxNum);
}

int ServerSocket::Accept(ClientSocket &client)
{
    sockaddr_in clientAddr;
    socklen_t len = sizeof(sockaddr);
    SOCKET clientSock = accept(m_sock,(sockaddr*)&clientAddr,&len);
    if (INVALID_SOCKET == clientSock)
    {
        return INVALID_SOCKET;
    }

    client.SetSocket(clientSock);

    SocketAddress sockAddr;
    sockAddr.SetIP(inet_ntoa(clientAddr.sin_addr));
    sockAddr.SetPort(ntohs(clientAddr.sin_port));
    client.SetLocalAddress(sockAddr);

    return 0;
}
