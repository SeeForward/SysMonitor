#ifndef __IP_ADDRESS_H__
#define __IP_ADDRESS_H__

#include <string>

class IPAddress
{
public:
    IPAddress(const std::string &ip);

    bool Valid() const;

    const std::string& Str() const;

private:
    bool IsValid();

private:
    std::string m_ip;
    bool        m_vaild;
};

#endif //__IP_ADDRESS_H__
