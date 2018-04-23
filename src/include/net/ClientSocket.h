#ifndef __CLIENT_SOCKET_H__
#define __CLIENT_SOCKET_H__

#include "Socket.h"

class ClientSocket : public Socket
{
public:
    ClientSocket();
    ~ClientSocket();

    int Connect(const SocketAddress &remote);
};


#endif //__CLIENT_SOCKET_H__
