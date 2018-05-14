#include "NetInfo.h"

#ifdef __WINDOWS__
#	include <stdlib.h>
#	include <winsock2.h>
#	include <iphlpapi.h>
#	include <psapi.h>
#else
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

using std::vector;
using std::string;

bool AdapterInfo::GetInfos(vector<AdapterInfo> &vecAi) 
{
	vecAi.clear();
#ifdef __WINDOWS__

	MIB_IFTABLE* pMIT = NULL;
	DWORD dwIfBufSize = 0; 
	if (ERROR_INSUFFICIENT_BUFFER != ::GetIfTable(pMIT, &dwIfBufSize, 0)) {
		return false;
	}
	
	pMIT = new MIB_IFTABLE[dwIfBufSize];
	if (NULL == pMIT) {
		return false;
	}
	
	
	if (NO_ERROR != ::GetIfTable(pMIT, &dwIfBufSize, 0)) {
		return false;
	}
	//mapping of adaptor's index and speed
	map<int, int64_t> mapIndexSpeed;

	for (size_t i = 0; i < pMIT->dwNumEntries; ++i) 
	{
		mapIndexSpeed[pMIT->table[i].dwIndex] = pMIT->table[i].dwSpeed; ; 
	}	
	delete[] pMIT;
	pMIT = NULL;

	PIP_ADAPTER_ADDRESSES pAddrs = NULL;
	ULONG bufLen = 0;
	if (ERROR_BUFFER_OVERFLOW != ::GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddrs, (DWORD*)&bufLen)) {
		return false;
	}
	//adapter changed, update the mapping of adapter's index and name
	if (bufLen != m_bufLen) {
		Destory();
		Init();
	}

	pAddrs = (PIP_ADAPTER_ADDRESSES)new char[m_bufLen];
	if (NULL == pAddrs) {
		return false;
	}
	
	if (NO_ERROR != ::GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddrs, (DWORD*)&bufLen)) {
		delete[] pAddrs;
		return false;
	}

	map<int, int64_t>::iterator iter = mapIndexSpeed.end();
	AdapterInfo ai;
	char buf[256];
	PIP_ADAPTER_ADDRESSES pCurAddr = pAddrs;
	while (pCurAddr) 
	{
		//ingore loopback address
		if (pCurAddr->IfType == 24) {
			//Next adapter
			pCurAddr = pCurAddr->Next;
			continue;
		}
		//name
		if (!::WideCharToMultiByte(CP_UTF8, NULL, pCurAddr->FriendlyName, (int)wcslen(pCurAddr->FriendlyName) + 1, buf, 256, 0, 0)) {
			ai.m_name = "";
		} else {
			ai.m_name = buf;
		}
		//desc
		if (!::WideCharToMultiByte(CP_UTF8, NULL, pCurAddr->Description, (int)wcslen(pCurAddr->Description) + 1, buf, 256, 0, 0)) {
			ai.m_desc = "";
		} else {
			ai.m_desc = buf;
		}
		//physical adddress
		if (pCurAddr->PhysicalAddressLength) 
		{
			char cp[256] = {0};
			char* pTmp = cp;
			for (int i = 0; i < (int)pCurAddr->PhysicalAddressLength; ++i) 
			{
				if (i == (pCurAddr->PhysicalAddressLength - 1)) {
					sprintf(pTmp, "%.2X",(int)pCurAddr->PhysicalAddress[i]);
				} else {
					sprintf(pTmp,"%.2X-",(int)pCurAddr->PhysicalAddress[i]);
					pTmp += strlen(pTmp);
				}
			}
			ai.m_physAddr = cp;
		}

		//ip address
		ai.m_ipAddrs.clear();
		PIP_ADAPTER_UNICAST_ADDRESS pIpAddr = pCurAddr->FirstUnicastAddress;
		char bufTmp[64];
		while (pIpAddr) 
		{
			sprintf(bufTmp,"%s",inet_ntoa(((SOCKADDR_IN*)(pIpAddr->Address.lpSockaddr))->sin_addr));
			ai.m_ipAddrs.push_back(bufTmp);
			pIpAddr = pIpAddr->Next;
		}

		//speed
		iter = mapIndexSpeed.find(pCurAddr->IfIndex);
		if (iter != mapIndexSpeed.end()) {
			ai.m_speed = iter->second / 1000000;
		} else {
			ai.m_speed = 0;
		}

		vecAi.push_back(ai);
		//next adapter
		pCurAddr = pCurAddr->Next;
	}

	if (pAddrs) {
		delete[] pAddrs;
		pAddrs = NULL;
	}
#else
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		return false;
	}

	//init ifconf
	char buf[10240];
	struct ifconf ifconf;
	ifconf.ifc_len = sizeof(buf);
	ifconf.ifc_buf = buf;


	if (ioctl(sockfd, SIOCGIFCONF, &ifconf) != 0) {
		close(sockfd);
		return false;
	}

	AdapterInfo ai;
	struct ifreq *pIfreq = (struct ifreq*)buf;
	for (int i = (ifconf.ifc_len / sizeof(struct ifreq)); i > 0; --i, ++pIfreq)
	{
		if (pIfreq->ifr_flags == AF_INET){ //for ipv4
			//ignore loopback
			if (strncmp(pIfreq->ifr_name, "lo", 2) == 0) {
				continue;
			}
			if (strncmp(pIfreq->ifr_name, "virbr", 5) == 0) {
				continue;
			}
			ai.m_name = pIfreq->ifr_name;
			ai.m_ipAddrs.clear();
			ai.m_ipAddrs.push_back(inet_ntoa(((struct sockaddr_in*)&(pIfreq->ifr_addr))->sin_addr));
			vecAi.push_back(ai);
		}
	}

	struct ifreq ifr;
	struct ethtool_cmd ec;
	for (size_t i = 0; i < vecAi.size(); ++i)
	{
		memset(&ifr, 0, sizeof(ifr));
		strncpy(ifr.ifr_name, vecAi[i].m_name.c_str(), sizeof(ifr.ifr_name) - 1);

		if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) != 0) {
			close(sockfd);
			return false;
		}
		//physical address
		char cp[256] = {0};
		char* pTmp = cp;
		for (int j = 0; j < 6; ++j) 
		{
			if (j == 5) {
				sprintf(pTmp, "%.2X",(unsigned char)ifr.ifr_hwaddr.sa_data[j]);
			} else {
				sprintf(pTmp,"%.2X-",(unsigned char)ifr.ifr_hwaddr.sa_data[j]);
				pTmp += strlen(pTmp);
			}
		}
		vecAi[i].m_physAddr = cp;

		//speed
		ec.cmd = ETHTOOL_GSET;
		ifr.ifr_data = (caddr_t)&ec;
		if (ioctl(sockfd, SIOCETHTOOL, &ifr) != 0) {
			close(sockfd);
			continue;
		}
		vecAi[i].m_speed = ec.speed;
	}
	close(sockfd);

	FILE *fp = popen("lspci 2> /dev/null | grep -E 'Ethernet|Network'", "r");
	if (!fgets(buf, sizeof(buf), fp)) 
	{
		pclose(fp);
		fp = popen("/sbin/lspci 2> /dev/null | grep -E 'Ethernet|Network'", "r");
		if (!fgets(buf, sizeof(buf), fp)) {
			pclose(fp);
			return true;
		}
	}
	string desc;
	size_t i = 0;
	do
	{
		desc = buf;
		int posBegin = desc.find_first_of(':');
		posBegin = desc.find_first_of(':', posBegin + 1);
		int posEnd = desc.find_first_of('(');
		if(i < vecAi.size()) {
			vecAi[i++].m_desc = desc.substr(posBegin + 2, posEnd - posBegin - 3);
		}
	} while (fgets(buf, sizeof(buf), fp));

	pclose(fp);
#endif //__WINDOWS__
	return true;
}

