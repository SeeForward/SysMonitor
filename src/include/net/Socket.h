#ifndef __I_SOCKET_H__
#define __I_SOCKET_H__

#include "TypeDef.h"
#include "SocketAddress.h"

class Socket
{
public:
    Socket();
    virtual ~Socket();

    void SetSocket(SOCKET sock);
    void SetLocalAddress(const SocketAddress &local);
    void SetRemoteAddress(const SocketAddress &remote);

    const SocketAddress& LocalAdderss() const;
    const SocketAddress& RemoteAdderss() const;

    int Send(uint8_t *buf, int len);
    int Recv(uint8_t *buf, int len);

protected:
    int           m_protocol;
    SOCKET        m_sock;
    SocketAddress m_local;
    SocketAddress m_remote;
};

#endif //__I_SOCKET_H__

