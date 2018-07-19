#include "Listener.h"
#include "Server.h"
#include "ServerCenter.h"
#include <boost/bind.hpp>

#include <iostream>

using namespace std;

void Listener::Accept()
{
    SocketType *pSock = new SocketType(m_ios);
    m_acceptor.async_accept(*pSock, boost::bind(&Listener::AcceptHandler, this, ErrorPlaceholder, pSock));
}

void Listener::AcceptHandler(const ErrorCode &ec, SocketType *pSock)
{
    if (ec)
    {
        cout << ec.value() << endl;
        cout << ec.category().name() << endl;
        return;
    }

    uint32_t size = 0;
    int rc = pSock->read_some(AsioBuffer((char*)&size, sizeof(size)));
    if (rc != sizeof(size))
    {
        cout << ec.value() << endl;
        cout << ec.category().name() << endl;
        Accept();
        return;
    }

    uint8_t *pRecvData = new uint8_t[size];
    rc = read(*pSock, AsioBuffer(pRecvData, size));
    if (rc != size)
    {
        Accept();
        return;
    }

    NetMessage negoMsg;
    negoMsg.FromStr(std::string((char*)pRecvData, size));

    cout << pSock->remote_endpoint().address() << endl;

    std::string s;
    negoMsg.ToStr(s);
    cout << "recv:" << s << endl;

    SocketPtr spSock(pSock);
    SmartPtr<Server> spServer = Singleton<ServerCenter>::Instance().Get(negoMsg.Code(), spSock);
     cout << "create server" << endl;
    if (!spServer)
    {
        cout << "server null" << endl;
    }
    else
    {
        NetMessagePtr pMsg = new NetMessage(1, "nego");
        string str;
        pMsg->ToStr(str);
        size = str.size();
        uint8_t *pData = new uint8_t[size + sizeof(uint32_t)];
        memcpy(pData, &size, sizeof(uint32_t));
        memcpy(pData + sizeof(uint32_t), str.c_str(), size); 
        spServer->Send(pData, size + sizeof(uint32_t));
    }

    cout << "accept again" << endl;

    Accept();
}

