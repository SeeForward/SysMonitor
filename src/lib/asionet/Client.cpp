#include "Client.h"
#include <iostream>

using namespace std;

int Client::Connect(const std::string &ip, int port)
{
    return Connect(AddressType::from_string(ip), port);
}

int Client::Connect(const AddressType &addr, int port)
{
    return Connect(EndpointType(addr, port));
}

int Client::Connect(const EndpointType &ep)
{
    ErrorCode ec;
    m_spSock->connect(ep, ec);
    if (ec)
    {
        cout << ec.value() << endl;
        cout << ec.category().name() << endl;
        return -ec.value();
    }
    Nego();
    AsyncRecv();
    return 0;
}

int Client::Nego()
{
	NetMessagePtr pNegoMsg = new NetMessage(1, "nego");

    string str;
    pNegoMsg->ToStr(str);
    uint32_t size = str.size();
    uint8_t *pData = new uint8_t[size + sizeof(uint32_t)];
    memcpy(pData, &size, sizeof(uint32_t));
    memcpy(pData + sizeof(uint32_t), str.c_str(), size); 
    return Send(pData, size + sizeof(uint32_t));
}

void Client::OnData(uint8_t *pData, uint32_t len)
{
        string str((char*)pData + sizeof(len), len - sizeof(len));
        cout << "recv data" << endl;
        cout << str<< endl;
}
/*
int Client::OnError(ErrorCode &ec)
{
        return ;
}*/
