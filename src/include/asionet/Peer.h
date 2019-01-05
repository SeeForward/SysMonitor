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
    bool Send(uint8_t *pData, uint32_t len);

    bool Vaild();
    void Close();

protected:
   virtual void SendHandler(uint8_t *pData, uint32_t len, const ErrorCode &ec);

    virtual void AsyncRecv();
    virtual void HeadRecvHandler(const ErrorCode &ec);
    virtual void BodyRecvHandler(const ErrorCode &ec);

    virtual void OnData(uint8_t *pData, uint32_t len);

    virtual int OnError(const ErrorCode &ec);
protected:
    SocketPtr  m_spSock;

    uint8_t   *m_pBuf;
    uint32_t   m_bufLen;
    uint32_t   m_bodyLen;
};

#endif //__PEER_H__

