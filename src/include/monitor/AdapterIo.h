//Date:2017-3-20
//Author:lidengke
//Desc:query the state of adapter


#ifndef __ADAPTER_IO_H__
#define __ADAPTER_IO_H__

#include "TypeDef.h"
#include "SystemTime.h"
#include "NetInfo.h"
#include "TcpInode.h"

#include <map>
#include <string>
#include <vector>

class AdapterIo 
{
public:
	AdapterIo() 
	{
		m_recv = 0;
		m_send = 0;
	}

public:
	std::string m_name;
	uint64_t    m_recv;	//(byte/s)
	uint64_t    m_send;	//(byte/s)
};

class Adapter 
{
public:
	Adapter();
	~Adapter();

	//get the send and recv io of adapters; total was put at the first
	bool GetIo(std::vector<AdapterIo> &vecAda);

private:
	bool Init();
	void Destory();

	bool GetIoCounts(std::vector<AdapterIo> &vecAs);

private:

#ifdef __WINDOWS__
	int32_t m_bufLen;
	void* m_pAddrs;
	// the mapping of adapter's index and name
	std::map<int, std::string> m_mapIndexName;
#endif //__WINDOWS__

	SystemTime m_lastIoTime;
	std::map<std::string, AdapterIo> m_mapNameState;
};

#endif //__NETWORK_STATE_H__

