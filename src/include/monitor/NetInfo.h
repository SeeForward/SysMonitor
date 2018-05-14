#ifndef __NET_INFO_H__
#define __NET_INFO_H__

#include "TypeDef.h"
#include <string>
#include <vector>

class AdapterInfo 
{
public:
	AdapterInfo() 
	{
		m_speed = 0;
	}

	//get information of all adapters
	static bool GetInfos(std::vector<AdapterInfo> &vecAi);

public:
	std::string m_name;
	std::string m_desc;
	std::string m_physAddr;
	uint64_t    m_speed;		//(Mbit/s)
	std::vector<std::string> m_ipAddrs;
};

#endif //__NETWORK_INFO_H__

