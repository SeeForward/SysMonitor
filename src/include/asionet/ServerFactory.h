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
        m_type = -1;
    }

    SocketPtr m_spS;
    uint32_t  m_id;
    uint32_t  m_type;
};

class ServerFactory
{
    friend class Singleton<ServerFactory>;
public:
    bool Add(SocketPtr spSock);
    
    bool Register(uint32_t serverType, CREATEFUNC create);

private:
    void AsyncRecvType(SocketCtx *pSC);
    void TypeRecvHandler(SocketCtx *pSC, const ErrorCode &ec);

    void CreateServer(SocketCtx &sc);

    int OnError(const ErrorCode &ec);

private:
    ServerFactory() { m_id = 0;}

private:
    // TODO : add mutex
    uint32_t m_id;
    std::map<uint32_t, SocketCtx>  m_socks;
    std::map<uint32_t, CREATEFUNC> m_entrance;
};

#endif //__SERVER_FACTORY_H__

