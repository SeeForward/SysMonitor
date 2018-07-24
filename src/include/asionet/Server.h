#ifndef __SERVER_H__
#define __SERVER_H__

#include "Peer.h"
#include "NetMessage.h"
#include <iostream>

using namespace std;

class Server : public Peer
{
public:
    Server(SocketPtr spSock);

    virtual ~Server() {}

    void SetId(uint32_t id) {m_id = id;}
    uint32_t GetId() {return m_id;}

protected:

    void OnData(uint8_t *pData, uint32_t len);

private:
    uint32_t m_id;
};

Server* CreateServer(SocketPtr spSock);

typedef SmartPtr<Server> ServerPtr;

#endif //__SERVER_H__ 

