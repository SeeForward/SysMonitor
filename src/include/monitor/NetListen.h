//Date:2017-3-6
//Auther:lidengke
//Desc:query the listen state of process

#ifndef __NET_LISTEN__
#define __NET_LISTEN__

#include "TypeDef.h"
#include "TcpInode.h"
#include <string>
#include <vector>

#define	NS_PROT_TCP	1	//TCP protocal
#define NS_PROT_UDP	2	//UDP protocal

//the listening port of process
class ListenProc 
{
public:
	ListenProc() 
	{
		m_pid = -1;
		m_localPort = 0;
	}

	//get the listening port of process
	static bool Get(std::vector<ListenProc> &vecPls);

public:
	int32_t     m_pid;
	int32_t     m_type;	//the type of protocol (NS_PROT_TCP:TCP) (NS_PROT_TCP:UDP)
	std::string m_name;
	std::string m_localAddr;
	int32_t     m_localPort;
};

#endif //__NET_LISTEN__

