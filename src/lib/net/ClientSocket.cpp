#include "ClientSocket.h"

#ifdef __WINDOWS__
#include<winsock2.h>
#else //__LINUX__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

ClientSocket::ClientSocket()
{
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
}

ClientSocket::ClientSocket(const SocketAddress &local)
{
    m_local = local;
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
}

ClientSocket::~ClientSocket() {}

int ClientSocket::Connect(const SocketAddress &remote)
{
    m_remote = remote;
    sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = inet_addr(m_remote.IP().Str().c_str());
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_remote.Port());

    return connect(m_sock, (sockaddr*)&serverAddr, sizeof(sockaddr));
}
