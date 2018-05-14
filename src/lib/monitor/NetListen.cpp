#include "NetListen.h"

#include "ProcessName.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/ethtool.h>

using std::vector;
using std::string;

bool ListenProc::Get(vector<ListenProc> &vecPls) 
{
	vecPls.clear();
#ifdef __WINDOWS__
	//get the listening port of tcp
	MIB_TCPTABLE_OWNER_PID* mtop = NULL;
	DWORD dwSize = 0;
	if (ERROR_INSUFFICIENT_BUFFER != ::GetExtendedTcpTable(mtop, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_LISTENER, 0)) 
	{
		return false;
	}

	mtop = (MIB_TCPTABLE_OWNER_PID*)new char[dwSize];
	if (NULL == mtop) 
	{
		return false;
	}

	if (NO_ERROR != ::GetExtendedTcpTable(mtop, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_LISTENER, 0)) 
	{
		delete[] mtop;
		return false;
	}
	ProcessListenProc plsTmp;
	char pAddrs[64];
	for (size_t i = 0; i < mtop->dwNumEntries; ++i) 
	{
		plsTmp.m_pid = mtop->table[i].dwOwningPid;
		plsTmp.m_name = GetNameByPid(plsTmp.m_pid);
		plsTmp.m_type = NS_PROT_TCP;
		plsTmp.m_localPort = ntohs((unsigned short)mtop->table[i].dwLocalPort);

		//local ip address
		SOCKADDR_IN addrSrv;
		addrSrv.sin_addr.S_un.S_addr = mtop->table[i].dwLocalAddr;
		sprintf(pAddrs,"%s",inet_ntoa(addrSrv.sin_addr));
		plsTmp.m_localAddr = pAddrs;

		vecPls.push_back(plsTmp);
	}
	delete[] mtop;

	//get the listening port of udp
	MIB_UDPTABLE_OWNER_PID* muop = NULL;
	dwSize = 0;
	if (ERROR_INSUFFICIENT_BUFFER != ::GetExtendedUdpTable(muop, &dwSize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0)) 
	{
		return false;
	}

	muop = (MIB_UDPTABLE_OWNER_PID*)new char[dwSize];
	if (NULL == muop) 
	{
		return false;
	}

	if (NO_ERROR != ::GetExtendedUdpTable(muop, &dwSize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0)) 
	{
		delete[] muop;
		return false;
	}

	for(size_t i = 0; i < muop->dwNumEntries; ++i) 
	{
		plsTmp.m_pid = muop->table[i].dwOwningPid;
		plsTmp.m_name = GetNameByPid(plsTmp.m_pid);
		plsTmp.m_type = NS_PROT_UDP; 
		plsTmp.m_localPort = ntohs((unsigned short)muop->table[i].dwLocalPort);

		//local ip address
		SOCKADDR_IN addrSrv;
		addrSrv.sin_addr.S_un.S_addr = muop->table[i].dwLocalAddr;
		sprintf(pAddrs,"%s",inet_ntoa(addrSrv.sin_addr));
		plsTmp.m_localAddr = pAddrs;

		vecPls.push_back(plsTmp);
	}
	delete[] muop;
#else
	//get the listening port of tcp
	FILE *fp = fopen("/proc/net/tcp", "r");
	if (!fp) 
	{
		return false;
	}
	char buf[256];
	ListenProc plsTmp;
	//skip head line
	if (!fgets(buf, sizeof(buf), fp)) 
	{
		fclose(fp);
		return false;
	}
	//placeholder
	char c1;
	struct sockaddr_in localAddr;
	uint64_t inode = 0;
	char pAddr[64];
	int32_t state = 0;
	while (fgets(buf, sizeof(buf), fp)) 
	{
		sscanf(buf, "%*s %x%c%x %*s %x %*s %*s %*s %*s %*s %"PRIu64, 
					&(localAddr.sin_addr.s_addr), &c1, &plsTmp.m_localPort, &state, &inode);

		//ignore not listening
		if (state != 10) 
		{
			continue;
		}
		plsTmp.m_type = NS_PROT_TCP;

		sprintf(pAddr,"%s",inet_ntoa(localAddr.sin_addr));
		plsTmp.m_localAddr = pAddr;
		
		plsTmp.m_pid = TcpInode::Inst()->GetTcpPidByInode(inode);
		plsTmp.m_name = Singleton<ProcessName>::Instance().GetNameByPid(plsTmp.m_pid);
		vecPls.push_back(plsTmp);
	}
	fclose(fp);

	//get the listening port of udp
	fp = fopen("/proc/net/udp", "r");
	if (!fp) 
	{
		return false;
	}
	//ignore head line
	if (!fgets(buf, sizeof(buf), fp)) 
	{
		return false;
	}
	while (fgets(buf, sizeof(buf), fp)) 
	{
		sscanf(buf, "%*s %x%c%x %*s %x %*s %*s %*s %*s %*s %"PRIu64, 
					&(localAddr.sin_addr.s_addr), &c1, &plsTmp.m_localPort, &state, &inode);

		//ignore not listening
		if (state != 7) 
		{
			continue;
		}
		plsTmp.m_type = NS_PROT_UDP;

		sprintf(pAddr,"%s",inet_ntoa(localAddr.sin_addr));
		plsTmp.m_localAddr = pAddr;
		
		plsTmp.m_pid = TcpInode::Inst()->GetTcpPidByInode(inode);
		plsTmp.m_name = Singleton<ProcessName>::Instance().GetNameByPid(plsTmp.m_pid);
		vecPls.push_back(plsTmp);
	}
	fclose(fp);
#endif //__WINDOWS__
	return true;
}

