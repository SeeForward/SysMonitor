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
    m_remote = remote;

    // server ip address
    sockaddr_in serverAddr;
    serverAddr.sin_addr.s_addr = inet_addr(remote.IP().Str().c_str());
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(remote.Port());

    int rc = connect(m_sock, (sockaddr*)&serverAddr, sizeof(sockaddr));
    if (0 == rc)
    {
        // remote ip address
        sockaddr_in remoteAddr;
        socklen_t len = sizeof(sockaddr_in);
        int rc2 = getpeername(m_sock, (sockaddr*)&remoteAddr, &len);
        if (0 == rc2)
        {
            m_remote.SetIP(inet_ntoa(remoteAddr.sin_addr));
            m_remote.SetPort(ntohs(remoteAddr.sin_port));
        }

        // local ip address
        sockaddr_in localAddr;
        len = sizeof(sockaddr_in);
        rc2 = getsockname(m_sock, (sockaddr*)&localAddr, &len);
        if (0 == rc2)
        {
            m_local.SetIP(inet_ntoa(localAddr.sin_addr));
            m_local.SetPort(ntohs(localAddr.sin_port));
        }
    }
    return rc;
}
