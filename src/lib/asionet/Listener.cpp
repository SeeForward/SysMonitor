#include "Listener.h"
#include "Server.h"
#include "ServerCenter.h"
#include "ServerFactory.h"
#include <boost/bind.hpp>

#include <iostream>

using namespace std;

void Listener::Accept()
{
    SocketType *pSock = new SocketType(m_ios);
    m_acceptor.async_accept(*pSock, boost::bind(&Listener::AcceptHandler, this, ErrorPlaceholder, pSock));
}

void Listener::AcceptHandler(const ErrorCode &ec, SocketType *pSock)
{
    if (ec)
    {
        //OnError(ec);
        return;
    }
    else
    {
        SocketPtr spSoc(pSock);
        Singleton<ServerFactory>::Instance().Add(spSoc);
    }
    Accept();
    return ;

}

