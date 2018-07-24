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
    uint32_t serverType = 1;
    uint32_t *pType = new uint32_t(serverType); 
    return Send((uint8_t*)pType, NET_HEAD_LEN);
}

void Client::OnData(uint8_t *pData, uint32_t len)
{
    string str((char*)pData + NET_HEAD_LEN, len - NET_HEAD_LEN);
    cout << "recv data" << endl;
    cout << str<< endl;
}

/*
int Client::OnError(ErrorCode &ec)
{
        return ;
}*/
