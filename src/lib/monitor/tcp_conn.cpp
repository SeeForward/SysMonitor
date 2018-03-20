#include "tcp_conn.h"
#include "system_tool.h"
#include "process_name.h"

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

using std::vector;
using std::string;

bool TcpConn::Get(vector<TcpConn> &vecPtc) 
{
	vecPtc.clear();
#ifdef __WINDOWS__
	MIB_TCPTABLE_OWNER_PID* mtop = NULL;
	DWORD dwSize = 0;
	if (ERROR_INSUFFICIENT_BUFFER != ::GetExtendedTcpTable(mtop, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) 
	{
		return false;
	}

	mtop = (MIB_TCPTABLE_OWNER_PID*)new char[dwSize];
	if (NULL == mtop) 
	{
		return false;
	}
	if (NO_ERROR != ::GetExtendedTcpTable(mtop, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) 
	{
		delete[] mtop;
		return false;
	}

	TcpConn ptcTmp;
	char pAddrs[64];
	for (size_t i = 0; i < mtop->dwNumEntries; ++i) 
	{
		ptcTmp.m_pid = mtop->table[i].dwOwningPid;
		ptcTmp.m_name = GetNameByPid(ptcTmp.m_pid );
		
		//ignore tcp when is listening
		if(mtop->table[i].dwState == MIB_TCP_STATE_LISTEN) 
		{
			continue;
		}
		ptcTmp.m_state = TcpStateUniform(mtop->table[i].dwState);

		ptcTmp.m_localPort = ntohs((unsigned short)mtop->table[i].dwLocalPort);
		ptcTmp.m_remotePort = ntohs((unsigned short)mtop->table[i].dwRemotePort);

		//local ip address
		SOCKADDR_IN addrSrv;
		addrSrv.sin_addr.S_un.S_addr = mtop->table[i].dwLocalAddr;
		sprintf(pAddrs,"%s",inet_ntoa(addrSrv.sin_addr));
		ptcTmp.m_localAddr = pAddrs;

		//remoate ip address
		addrSrv.sin_addr.S_un.S_addr = mtop->table[i].dwRemoteAddr;
		sprintf(pAddrs,"%s",inet_ntoa(addrSrv.sin_addr));
		ptcTmp.m_remoteAddr = pAddrs;

		vecPtc.push_back(ptcTmp);
	}
	delete[] mtop;
#else
	FILE *fp = fopen("/proc/net/tcp", "r");
	if (!fp) 
	{
		return false;
	}
	char buf[256];
	TcpConn ptcTmp;
	//skip head line
	if (!fgets(buf, sizeof(buf), fp)) 
	{
		fclose(fp);
		return false;
	}
	//placeholder
	char c1;
	char c2;
	struct sockaddr_in localAddr;
	struct sockaddr_in remoteAddr;
	uint64_t inode = 0;
	char pAddr[64];
	int32_t state = 0;
	while (fgets(buf, sizeof(buf), fp)) 
	{
		sscanf(buf, "%*s %x%c%x %x%c%x %x %*s %*s %*s %*s %*s %"PRIu64, 
					&(localAddr.sin_addr.s_addr), &c1, &ptcTmp.m_localPort,
					&(remoteAddr.sin_addr.s_addr), &c2, &ptcTmp.m_remotePort, &state, &inode);

		//ignore tcp when is listening
		if (state != 1) 
		{
			continue;
		}
		ptcTmp.m_state = TcpStateUniform(state);

		sprintf(pAddr,"%s",inet_ntoa(localAddr.sin_addr));
		ptcTmp.m_localAddr = pAddr;

		sprintf(pAddr,"%s",inet_ntoa(remoteAddr.sin_addr));
		ptcTmp.m_remoteAddr = pAddr;
		
		ptcTmp.m_pid = TcpInode::Inst()->GetTcpPidByInode(inode);
		ptcTmp.m_name = ProcessName::Inst()->GetNameByPid(ptcTmp.m_pid);
		vecPtc.push_back(ptcTmp);
	}

	fclose(fp);
#endif //__WINDOWS__
	return true;
}

int TcpStateUniform(int state) 
{
#ifdef __WINDOWS__
	if (state >= 1 && state <= 12) 
	{
		return state;
	}
#else
	if (state >= 1 && state <= 11) 
	{
		switch (state) 
		{
			case 1:
				return NS_TCP_ESTAB;
			case 2:
				return NS_TCP_SYN_SENT;
			case 3:
				return NS_TCP_SYN_RCVD;
			case 4:
				return NS_TCP_FIN_WAIT1;
			case 5:
				return NS_TCP_FIN_WAIT2;
			case 6:
				return NS_TCP_TIME_WAIT;
			case 7:
				return NS_TCP_CLOSED;
			case 8:
				return NS_TCP_CLOSE_WAIT;
			case 9:
				return NS_TCP_LAST_ACK;
			case 10:
				return NS_TCP_LISTEN;
			case 11:
				return NS_TCP_CLOSING;
		}
	}
#endif //__WINDOWS__
	return NS_TCP_UNKNOW;
}

