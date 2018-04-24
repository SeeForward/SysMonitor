#include "ServerSocket.h"

#ifdef __WINDOWS__
#include<winsock2.h>
#else //__LINUX__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

ServerSocket::ServerSocket(const SocketAddress &local) 
{
    m_local = local;
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
}

ServerSocket::~ServerSocket() {}

int ServerSocket::Listen(int maxNum)
{
    sockaddr_in localAddr;
    if (m_local.IP().Valid())
    {
        localAddr.sin_addr.s_addr = inet_addr(m_local.IP().Str().c_str());
    }
    else
    {
        localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(m_local.Port());
    int rc = bind(m_sock, (sockaddr*)&localAddr, sizeof(sockaddr));
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

    // remote ip address
    SocketAddress remoteAddr;
    remoteAddr.SetIP(inet_ntoa(clientAddr.sin_addr));
    remoteAddr.SetPort(ntohs(clientAddr.sin_port));
    client.SetRemoteAddress(remoteAddr);

    // local ip address
    sockaddr_in localAddr;
    len = sizeof(sockaddr_in);
    int rc = getsockname(m_sock, (sockaddr*)&localAddr, &len);
    if (0 == rc)
    {
        SocketAddress localAddrEx;
        localAddrEx.SetIP(inet_ntoa(localAddr.sin_addr));
        localAddrEx.SetPort(ntohs(localAddr.sin_port));
        client.SetLocalAddress(localAddrEx);
    }

    return 0;
}
