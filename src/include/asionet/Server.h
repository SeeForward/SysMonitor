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

    void OnData(uint8_t *pData, uint32_t len)
    {
            string str1((char*)pData + sizeof(len), len - sizeof(len));
            cout << "recv data" << endl;
            cout << str1 << endl;
            //NetMessagePtr pMsg = new NetMessage(1, "nego");
            //string str;
            //pMsg->ToStr(str);
            uint32_t size = str1.size();//str.size();
            uint8_t *pSendData = new uint8_t[size + sizeof(uint32_t)];
            memcpy(pSendData, &size, sizeof(uint32_t));
            memcpy(pSendData + sizeof(uint32_t), str1.c_str(), size); 
            Send(pSendData, size + sizeof(uint32_t));
            //Send(pData, len);
    }
};

Server* CreateServer(SocketPtr spSock);

typedef SmartPtr<Server> ServerPtr;

#endif //__SERVER_H__ 

