#ifndef __SERVER_H__
#define __SERVER_H__

#include "Peer.h"
#include "NetMessage.h"
#include <iostream>

using namespace std;

class Server : public Peer
{
public:
    Server(SocketPtr spSock) : Peer(spSock)
    {
        AsyncRecv();
        cout << "server construcnt" << endl;
    }

    virtual ~Server() {}

protected:

    void OnData(uint8_t *pData, uint32_t len);
};

Server* CreateServer(SocketPtr spSock);

typedef SmartPtr<Server> ServerPtr;

#endif //__SERVER_H__ 

