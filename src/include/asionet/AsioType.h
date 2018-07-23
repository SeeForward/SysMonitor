#ifndef __ASIO_TYPE_H__
#define __ASIO_TYPE_H__

#include <boost/asio.hpp>
#include "SmartPtr.h"

#define AsioIPv4             boost::asio::ip::tcp::v4()
#define AsioBuffer           boost::asio::buffer
#define ErrorPlaceholder     boost::asio::placeholders::error

#define NET_HEAD_LEN         sizeof(uint32_t)

typedef boost::system::error_code           ErrorCode;

typedef boost::asio::streambuf              StreamBufType;
typedef boost::asio::ip::address            AddressType;
typedef boost::asio::ip::tcp                TcpProtocolType;
typedef boost::asio::ip::tcp::endpoint      EndpointType;
typedef boost::asio::ip::tcp::socket        SocketType;
typedef boost::asio::ip::tcp::acceptor      AcceptorType;
typedef boost::asio::io_service             IOServiceType;

typedef SmartPtr<SocketType>                SocketPtr;

#endif //__ASIO_TYPE_H__
