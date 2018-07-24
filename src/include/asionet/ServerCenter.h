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
    void Add(ServerPtr pServer);

    void Del(ServerPtr pServer);

private:
    ServerCenter() { m_id = 0;}

private:
    // TODO : add mutex
    int m_id;

    std::map<int, ServerPtr> m_servers;
};


#endif //__SERVER_CENTER_H__
