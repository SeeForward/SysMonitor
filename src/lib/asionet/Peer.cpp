#include "Peer.h"
#include <boost/bind.hpp>

#include <iostream>

using namespace std;


Peer::Peer(IOServiceType &service) : m_spSock(new SocketType(service))
{
	//AsyncRecv();
}

Peer::Peer(SocketPtr spSock) : m_spSock(spSock)
{
	//AsyncRecv();
}

Peer::~Peer() {}

bool Peer::Vaild()
{
    return false;
}

void Peer::Close()
{
    return m_spSock->close();
}

bool Peer::Send(uint8_t *pData, uint32_t len)
{
    cout << "peer send" << endl;
    async_write(*m_spSock.Get(), AsioBuffer(pData, len),
                            boost::bind(&Peer::SendHandler, this, ErrorPlaceholder));

    return true;
}

void Peer::SendHandler(const ErrorCode &ec)
{
    if (ec)
    {
        cout << ec.value() << endl;
        cout << ec.category().name() << endl;
    }
    cout << "send ok" << endl;
    return ;
}

void Peer::AsyncRecv()
{
    cout << "peer recv" << endl;
    async_read(*m_spSock.Get(), AsioBuffer(&m_len, sizeof(m_len)), 
                    boost::bind(&Peer::HeadRecvHandler, this, ErrorPlaceholder));
}

void Peer::HeadRecvHandler(const ErrorCode &ec)
{
    cout << "recv head" << endl;
    if (ec)
    {
        Close();
        cout << ec.value() << endl;
        cout << ec.category().name() << endl;
        return ;
    }

    m_pData = new uint8_t[m_len + sizeof(m_len)];
    memcpy(m_pData + sizeof(m_len), &m_len, sizeof(m_len));
    async_read(*m_spSock.Get(), AsioBuffer(m_pData + sizeof(m_len), m_len),
                    boost::bind(&Peer::BodyRecvHandler, this, ErrorPlaceholder));
}

void Peer::BodyRecvHandler(const ErrorCode &ec)
{
    cout << "recv body" << endl;
    if (ec)
    {
        cout << ec.value() << endl;
        cout << ec.category().name() << endl;
        Close();
        return ;
    }

    OnData(m_pData, m_len + sizeof(m_len));

    async_read(*m_spSock.Get(), AsioBuffer(&m_len, sizeof(m_len)),
                    boost::bind(&Peer::HeadRecvHandler, this, ErrorPlaceholder));
}

