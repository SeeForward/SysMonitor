#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "Peer.h"
#include "AsioType.h"
#include "Message.h"


class Client : public Peer
{
public:
    Client(IOServiceType &service) : Peer(service) {}
    virtual ~Client() {} 

    int Connect(const std::string &ip, int port);
    int Connect(const AddressType &addr, int port);
    int Connect(const EndpointType &ep);

    virtual void OnData(uint8_t *pData, uint32_t len);

protected:
    int Nego();
};

#endif //__CLIENT_H__ 
