#include "Listener.h"
#include "ServerCenter.h"
#include "ServerFactory.h"

#include <iostream>

using namespace std;

int main(int argc, const char* argv[])
{

    Singleton<ServerFactory>::Instance().Register(1, CreateServer);

    IOServiceType ios;
    Listener listener(ios, 12345);

    listener.Accept();

    ios.run();

/*
    int port = 12345;
    TcpProtocolType tpt = IPv4;

    EndpointType  m_endpoint(tpt, port);
    AcceptorType  m_acceptor(ios, m_endpoint);

    SocketType *pSock = new SocketType(ios);
    m_acceptor.accept(*pSock);
*/
    return 0;
}
