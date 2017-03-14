#ifndef __TCP_CONN_H__
#define __TCP_CONN_H__

#include "type_def.h"
#include "tcp_inode.h"

#include <string>
#include <vector>

//TCP connection state
#define NS_TCP_UNKNOW           0
#define NS_TCP_CLOSED           1
#define NS_TCP_LISTEN           2
#define NS_TCP_SYN_SENT         3
#define NS_TCP_SYN_RCVD         4
#define NS_TCP_ESTAB            5
#define NS_TCP_FIN_WAIT1        6
#define NS_TCP_FIN_WAIT2        7
#define NS_TCP_CLOSE_WAIT       8
#define NS_TCP_CLOSING          9
#define NS_TCP_LAST_ACK         10
#define NS_TCP_TIME_WAIT        11
#define NS_TCP_DELETE_TCB       12

//tcp connnection
class TcpConn 
{
public:
	TcpConn() 
	{
		m_pid = -1;
		m_localPort = 0;
		m_remotePort = 0;
	}

	//get tcp connnections of process
	static bool Get(std::vector<TcpConn> &vecPtc);

public:
	int32_t     m_pid;
	int32_t     m_state;
	std::string m_name;
	std::string m_localAddr;
	std::string m_remoteAddr;
	int32_t     m_localPort;
	int32_t     m_remotePort;
};


//uniform the state of tcp connection (Windows and Linux)
int TcpStateUniform(int state);

#endif //__TCP_C0NN_H__

