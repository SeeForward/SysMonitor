#include "Socket.h"

#ifdef __WINDOWS__
#include<winsock2.h>
#else //__LINUX__
#include <unistd.h>
#include <sys/socket.h>
#endif

Socket::Socket() : m_sock(INVALID_SOCKET) { }

Socket::~Socket()
{
    close(m_sock);
}

void Socket::SetSocket(SOCKET sock)
{
    m_sock = sock;
}

void Socket::SetLocalAddress(const SocketAddress &local)
{
    m_local = local;
}

void Socket::SetRemoteAddress(const SocketAddress &remote)
{
    m_remote = remote;
}

const SocketAddress& Socket::LocalAdderss() const
{
    return m_local;
}

const SocketAddress& Socket::RemoteAdderss() const
{
    return m_remote;
}

int Socket::Send(uint8_t *buf, int len)
{
    return send(m_sock, buf, len, 0);
}

int Socket::Recv(uint8_t *buf, int len)
{
    return recv(m_sock, buf, len, 0);
}