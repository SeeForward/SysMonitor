//Date:2017-3-20
//Author:lidengke

#include "adapter_io.h"
#include "system_tool.h"

#ifdef __WINDOWS__
#	include <stdlib.h>
#	include <winsock2.h>
#	include <iphlpapi.h>
#	include <psapi.h>
#else //__LINUX__
#	include <stdio.h>
#	include <string.h>
#	include <dirent.h>
#	include <unistd.h>
#	include <sys/socket.h>
#	include <sys/ioctl.h>
#	include <sys/param.h> 
#	include <arpa/inet.h>
#	include <linux/if.h>
#	include <linux/sockios.h>
#	include <linux/ethtool.h>
#endif //__WINDOWS__

using std::map;
using std::vector;
using std::string;

Adapter::Adapter() 
{
#ifdef __WINDOWS__
	m_bufLen = 0;
	m_pAddrs = NULL;
#endif //__WINDOWS__

	Init();
}

Adapter::~Adapter() 
{
	Destory();
}

bool Adapter::GetIo(vector<AdapterIo> &vecAs) 
{
	SystemTime stCur = SystemTime::Now();
	vector<AdapterIo> vecAsCur;
	if (!GetIoCounts(vecAsCur)) 
	{
		return false;
	}

	vecAs.clear();
	vecAs.reserve(vecAsCur.size() + 1);

	int64_t ms = 0;
	if (m_lastIoTime.m_system != 0) 
	{
		ms = SystemTime::Escape(m_lastIoTime, stCur);
	}
	m_lastIoTime = stCur;

	map<string, AdapterIo>::iterator iter = m_mapNameState.end();
	AdapterIo asTmp;
	for (size_t i = 0; i < vecAsCur.size(); i++) 
	{	
		asTmp.m_name = vecAsCur[i].m_name;
		
		if (ms)
		{
			iter = m_mapNameState.find(asTmp.m_name);
			if (iter != m_mapNameState.end()) 
			{
				if (vecAsCur[i].m_send > iter->second.m_send) 
				{
					asTmp.m_send = (int64_t)((vecAsCur[i].m_send - iter->second.m_send) / (float)ms * 1000); 
				} 
				else 
				{
					asTmp.m_send = 0; 
				}
				if (vecAsCur[i].m_recv > iter->second.m_recv) 
				{
					asTmp.m_recv = (int64_t)((vecAsCur[i].m_recv - iter->second.m_recv) / (float)ms * 1000);
				} 
				else 
				{
					asTmp.m_recv = 0;
				}

				iter->second.m_send = vecAsCur[i].m_send;
				iter->second.m_recv = vecAsCur[i].m_recv;
				
				vecAs.push_back(asTmp);
				continue;
			}
		}
		m_mapNameState[asTmp.m_name] = vecAsCur[i];
		
		asTmp.m_send = 0;
		asTmp.m_recv = 0;
		vecAs.push_back(asTmp);
	}
	return true;
}

bool Adapter::Init() 
{
#ifdef __WINDOWS__
	if (m_pAddrs) 
	{
		return false;
	}
	if (ERROR_BUFFER_OVERFLOW != ::GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, (PIP_ADAPTER_ADDRESSES)m_pAddrs, (DWORD*)&m_bufLen)) 
	{
		return false;
	}
	
	m_pAddrs = new char[m_bufLen];
	if (NULL == m_pAddrs) 
	{
		return false;
	}
	
	if (NO_ERROR != ::GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, (PIP_ADAPTER_ADDRESSES)m_pAddrs, (DWORD*)&m_bufLen)) 
	{
		delete[] m_pAddrs;
		m_pAddrs = NULL;
		m_bufLen = 0;
		return false;
	}

	char buf[256];
	PIP_ADAPTER_ADDRESSES pCurAddr = (PIP_ADAPTER_ADDRESSES)m_pAddrs;
	while (pCurAddr) 
	{
		if (!::WideCharToMultiByte(CP_ACP, NULL, pCurAddr->FriendlyName, (int)wcslen(pCurAddr->FriendlyName) + 1, buf, sizeof(buf), 0, 0)) 
		{
			continue;
		}
		//ignore loopback address
		if (pCurAddr->IfType != 24) 
		{
			m_mapIndexName[pCurAddr->IfIndex] = buf;
		}
		//next
		pCurAddr = pCurAddr->Next;
	}
#endif //__WINDOWS__
	return true;
}

void Adapter::Destory() 
{
#ifdef __WINDOWS__
	delete[] m_pAddrs;
	m_pAddrs = NULL;
	m_bufLen = 0;

	m_mapIndexName.clear();
#else
#endif //__WINDOWS__
}

bool Adapter::GetIoCounts(vector<AdapterIo> &vecAs) 
{
	vecAs.clear();
	//put the total at the front
	AdapterIo as;
	as.m_name = "total";
	vecAs.push_back(as);

#ifdef __WINDOWS__
	PIP_ADAPTER_ADDRESSES pAddrs = NULL;
	ULONG bufLen = 0;
	if (ERROR_BUFFER_OVERFLOW != ::GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddrs, (DWORD*)&bufLen)) 
	{
		return false;
	}
	//adapter changed, update the mapping of adapter's index and name
	if (bufLen != m_bufLen) 
	{
		Destory();
		Init();
	}

	MIB_IFTABLE* pMIT = NULL;
	DWORD dwIfBufSize = 0; 
	if (ERROR_INSUFFICIENT_BUFFER != ::GetIfTable(pMIT,&dwIfBufSize,0)) 
	{
		return false;
	}
	pMIT = new MIB_IFTABLE[dwIfBufSize]; 
	if (NULL == pMIT) 
	{
		return false;
	}
 
	if (NO_ERROR!= ::GetIfTable(pMIT, &dwIfBufSize, 0)) 
	{
		return false;
	}

	map<int, string>::iterator itr = m_mapIndexName.end();
	for (size_t i = 0; i < pMIT->dwNumEntries; ++i) 
	{
		itr = m_mapIndexName.find(pMIT->table[i].dwIndex);
		if(itr == m_mapIndexName.end()) 
		{
			continue;
		}
		as.m_name = itr->second;
		as.m_send = pMIT->table[i].dwOutOctets; 
		as.m_recv = pMIT->table[i].dwInOctets;
		vecAs.push_back(as);
	}
	delete[] pMIT;
#else
	FILE* fp = fopen("/proc/net/dev", "r");
	if (!fp) 
	{
		return false;
	}
	char buf[512];	
	//skip two lines
	if (!fgets(buf, sizeof(buf), fp)) 
	{
		fclose(fp);
		return false;
	}
	if (!fgets(buf, sizeof(buf), fp)) 
	{
		fclose(fp);
		return false;
	}
	char nameBuf[128];
	char name[128];
	while (fgets(buf, sizeof(buf), fp))
	{
		char* pos = strchr(buf, ':');
		strncpy(nameBuf, buf, pos - buf);
		nameBuf[pos - buf] = '\0';
		sscanf(nameBuf, "%s", name);

		++pos;
		sscanf(pos, "%"PRIu64" %*d %*d %*d %*d %*d %*d %*d %"PRIu64, &as.m_recv, &as.m_send);
		//ignore loopback
		if (strncmp(name, "lo", 2) == 0) 
		{
			continue;
		}
		//ignore IPv6-in-IPv4 tunnel interfaces
		if (strncmp(name, "sit", 3) == 0) 
		{
			continue;
		}
		//ignore virtual adapter
		if (strncmp(name, "virbr", 5) == 0) 
		{
			continue;
		}

		as.m_name = name;
		vecAs.push_back(as);
	}

	fclose(fp);
#endif //__WINDOWS__
	//calculate the total io count
	for (size_t i = 1; i < vecAs.size(); ++i) 
	{
		vecAs[0].m_send += vecAs[i].m_send;
		vecAs[0].m_recv += vecAs[i].m_recv;
	}
	return true;
}

