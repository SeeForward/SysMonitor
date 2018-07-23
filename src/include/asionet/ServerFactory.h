#ifndef __SERVER_FACTORY_H__
#define __SERVER_FACTORY_H__

#include <set>
#include "Server.h"
#include "Singleton.h"

typedef Server* (*CREATEFUNC)(SocketPtr spSock);


struct SocketCtx
{
    SocketCtx()
    {
            m_id = 0;
            m_pData = NULL;
            m_len = 0;
    }

    SocketPtr m_spS;
    int       m_id;
    uint8_t  *m_pData;
    uint32_t  m_len;
};

class ServerFactory
{
    friend class Singleton<ServerFactory>;
public:
    bool Add(SocketPtr spSock);
    
    bool Register(int serverType, CREATEFUNC create);

private:
    void AsyncRecv(SocketCtx *pSC);
    void HeadRecvHandler(SocketCtx *pSC, const ErrorCode &ec);
    void BodyRecvHandler(SocketCtx *pSC, const ErrorCode &ec);

    void CreateServer(SocketCtx &sc);

    int OnError(const ErrorCode &ec);

private:
    ServerFactory() { m_id = 0;}

private:
    // TODO : add mutex
    int m_id;
    std::map<int, SocketCtx>  m_socks;
    std::map<int, CREATEFUNC> m_entrance;
};

#endif //__SERVER_FACTORY_H__

