#include "Peer.h"
#include <boost/bind.hpp>

#include <iostream>

using namespace std;


Peer::Peer(IOServiceType &service) : m_spSock(new SocketType(service))
{
    m_pData = NULL;
    m_len = 0;
}

Peer::Peer(SocketPtr spSock) : m_spSock(spSock) {}

Peer::~Peer() {cout << "peer desconsturct" << endl;}

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
    cout << "send len:" << *((uint32_t *)pData) << endl;
    async_write(*m_spSock.Get(), AsioBuffer(pData, len),
                            boost::bind(&Peer::SendHandler, this, pData, len, ErrorPlaceholder));

    return true;
}

void Peer::SendHandler(uint8_t *pData, uint32_t len, const ErrorCode &ec)
{
    delete[] pData;
    pData = NULL;
    cout << "Len :" << len << endl;
    if (ec)
    {
        OnError(ec);
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
        OnError(ec);
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
        OnError(ec);
        return ;
    }

    OnData(m_pData, m_len + sizeof(m_len));

    async_read(*m_spSock.Get(), AsioBuffer(&m_len, sizeof(m_len)),
                    boost::bind(&Peer::HeadRecvHandler, this, ErrorPlaceholder));
}

int Peer::OnError(const ErrorCode &ec)
{
    cout << ec.value() << endl;
    cout << ec.category().name() << endl;
    return -ec.value();
}
