#include "Peer.h"
#include <boost/bind.hpp>

#include <iostream>

using namespace std;


Peer::Peer(IOServiceType &service) : m_spSock(new SocketType(service))
{
    m_pBuf = NULL;
    m_bufLen = 0;
    m_bodyLen = 0;
}

Peer::Peer(SocketPtr spSock) : m_spSock(spSock)
{
    m_pBuf = NULL;
    m_bufLen = 0;
    m_bodyLen = 0;
}

Peer::~Peer()
{
    delete[] m_pBuf;
    m_bufLen = 0;
    m_bodyLen = 0;

    cout << "peer desconsturct" << endl;
}

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
    cout << "send body len:" << *((uint32_t *)pData) << endl;
    async_write(*m_spSock.Get(), AsioBuffer(pData, len),
                            boost::bind(&Peer::SendHandler, this, pData, len, ErrorPlaceholder));

    return true;
}

void Peer::SendHandler(uint8_t *pData, uint32_t len, const ErrorCode &ec)
{
    delete[] pData;
    pData = NULL;
    cout << "send sum len:" << len << endl;
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

    async_read(*m_spSock.Get(), AsioBuffer(&m_bodyLen, NET_HEAD_LEN), 
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
    cout << "recv body len:" << m_bodyLen << endl;

    int32_t len = m_bodyLen + NET_HEAD_LEN;
    cout << "recv sum len:" << len << endl;
    if (m_bufLen < len)
    {
        delete[] m_pBuf;
        m_pBuf = new uint8_t[len];
        m_bufLen = len;
    }

    memcpy(m_pBuf, &m_bodyLen, NET_HEAD_LEN);

    async_read(*m_spSock.Get(), AsioBuffer(m_pBuf + NET_HEAD_LEN, m_bodyLen),
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

    OnData(m_pBuf, m_bodyLen + NET_HEAD_LEN);

    async_read(*m_spSock.Get(), AsioBuffer(&m_bodyLen, NET_HEAD_LEN),
                    boost::bind(&Peer::HeadRecvHandler, this, ErrorPlaceholder));
}

void Peer::OnData(uint8_t *pData, uint32_t len) 
{
}

int Peer::OnError(const ErrorCode &ec)
{
    cout << ec.value() << endl;
    cout << ec.category().name() << endl;
    return -ec.value();
}
