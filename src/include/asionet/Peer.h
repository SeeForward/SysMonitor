#ifndef __PEER_H__
#define __PEER_H__

#include "AsioType.h"
#include "NetMessage.h"

class Peer
{
public:
    Peer(IOServiceType &service);
    Peer(SocketPtr spSock);
    virtual ~Peer();

    //bool Send(NetMessagePtr pMsg);
    //bool Send(NetBufferPtr pBuf);
    bool Send(uint8_t *pData, uint32_t len);

    bool Vaild();
    void Close();

protected:
public:
    virtual void SendHandler(const ErrorCode &ec);

    virtual void AsyncRecv();
    virtual void HeadRecvHandler(const ErrorCode &ec);
    virtual void BodyRecvHandler(const ErrorCode &ec);

    //virtual void OnMessage(NetMessagePtr pMsg) {}
    //virtual void OnMessage(NetBufferPtr pBuf) {}
    virtual void OnData(uint8_t *pData, uint32_t len) {}

protected:
public:
    SocketPtr  m_spSock;
    //NetBuffer  m_sendBuffer;
    //NetBuffer  m_recvBuffer;
    uint8_t   *m_pData;
    uint32_t   m_len;
};

#endif //__PEER_H__

