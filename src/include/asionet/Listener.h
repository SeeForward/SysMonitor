#ifndef __LISTENER_H__
#define __LISTENER_H__

#include "AsioType.h"


class Listener
{
public:
    Listener(IOServiceType &ios, int port, TcpProtocolType tpt = AsioIPv4) : 
        m_ios(ios), m_endpoint(tpt, port), m_acceptor(ios, m_endpoint) {}

    void Accept();

private:
    void AcceptHandler(const ErrorCode &ec, SocketType *pSock);

private:
    IOServiceType &m_ios;
    EndpointType   m_endpoint;
    AcceptorType   m_acceptor;
};

#endif //__LISTENER_H__
