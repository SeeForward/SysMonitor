#ifndef __SERVER_SOCKET_H__
#define __SERVER_SOCKET_H__

#include "ClientSocket.h"


class ServerSocket : public Socket
{
public:
    ServerSocket(const SocketAddress &local);
    ~ServerSocket();
    
    int Listen(int maxNum);

    int Accept(ClientSocket& client);
};

#endif //__SERVER_SOCKET_H__
