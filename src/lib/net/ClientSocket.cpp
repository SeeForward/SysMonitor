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

ClientSocket::~ClientSocket() {}

int ClientSocket::Connect(const SocketAddress &remote)
{
    // remote ip address
    m_remote = remote;
    sockaddr_in remoteAddr;
    remoteAddr.sin_addr.s_addr = inet_addr(m_remote.IP().Str().c_str());
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(m_remote.Port());

    int rc = connect(m_sock, (sockaddr*)&remoteAddr, sizeof(sockaddr));
    if (0 == rc)
    {
        // local ip address
        sockaddr_in localAddr;
        socklen_t len = sizeof(sockaddr_in);
        int rc2 = getsockname(m_sock, (sockaddr*)&localAddr, &len);
        if (0 == rc2)
        {
            m_local.SetIP(inet_ntoa(localAddr.sin_addr));
            m_local.SetPort(localAddr.sin_port);
        }
    }
    return rc;
}
