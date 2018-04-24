#ifndef __SOCKET_ADDRESS_H__
#define __SOCKET_ADDRESS_H__

#include "IPAddress.h"

class SocketAddress
{
public:
    SocketAddress();
    SocketAddress(const SocketAddress& other);
    SocketAddress(const std::string &ip, int port);
    SocketAddress(const IPAddress &ip, int port);

    int SetIP(const std::string &ip);
    int SetIP(const IPAddress &ip);
    void SetPort(int port);

    const IPAddress& IP() const;
    int Port() const;

    std::string ToStr() const;

private:
    IPAddress m_ip;
    int       m_port;
};

#endif //__SOCKET_ADDRESS_H__
