#include "SocketAddress.h"
#include "StringUtil.h"

SocketAddress::SocketAddress() : m_port(0) { }

SocketAddress::SocketAddress(const SocketAddress& other)
{
    m_ip = other.m_ip;
    m_port = other.m_port;
}

SocketAddress::SocketAddress(const std::string &ip, int port) : m_ip(ip), m_port(port) { }

SocketAddress::SocketAddress(const IPAddress &ip, int port) : m_ip(ip), m_port(port) { }

int SocketAddress::SetIP(const std::string &ip)
{
    m_ip = IPAddress(ip);
}

int SocketAddress::SetIP(const IPAddress &ip)
{
    m_ip = ip;
}

void SocketAddress::SetPort(int port)
{
    m_port = port;
}

const IPAddress& SocketAddress::IP() const
{
    return m_ip;
}

int SocketAddress::Port() const
{
    return m_port;
}

std::string SocketAddress::ToStr() const
{
    return m_ip.Str() + ":" + StringUtil::ToStr(m_port);
}
