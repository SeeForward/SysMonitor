#ifndef __SERVER_CENTER_H__
#define __SERVER_CENTER_H__

#include <set>
#include "Server.h"
#include "Singleton.h"

typedef Server* (*CREATEFUNC)(SocketPtr spSock);

class ServerCenter
{
    friend class Singleton<ServerCenter>;
public:

public:
    ServerPtr Get(int serverType, SocketPtr spSock);

    void Del(ServerPtr pServer)
    {
        
    }

    bool Register(int serverType, CREATEFUNC create);

private:
    void Add(ServerPtr pServer)
    {
        m_servers[m_id] = pServer;
        ++m_id;
    }

private:
    ServerCenter() { m_id = 0;}

private:
    // TODO : add mutex
    int m_id;

    std::map<int, ServerPtr> m_servers;

    std::map<int, CREATEFUNC> m_entrance;
};


#endif //__SERVER_CENTER_H__
