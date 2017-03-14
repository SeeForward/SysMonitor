#include "tcp_inode.h"

#ifdef __LINUX__

#include "system_tool.h"

#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include <vector>

using std::map;
using std::vector;

TcpInode::TcpInode() {}

TcpInode* TcpInode::Inst()
{
	static TcpInode ti;
	return &ti;
}

int32_t TcpInode::GetTcpPidByInode(int64_t inode) 
{
	if (inode <= 0) 
	{
		return -1;
	}

	if (m_mapInodePid.empty()) 
	{
		if (!GetInodePidMap(m_mapInodePid)) 
		{
			return -1;
		}
	}

	map<int64_t, int32_t>::iterator iter = m_mapInodePid.find(inode);
	if (iter == m_mapInodePid.end()) 
	{
		if (!GetInodePidMap(m_mapInodePid)) 
		{
			return -1;
		}
		else 
		{
			iter = m_mapInodePid.find(inode);
			if (iter == m_mapInodePid.end()) 
			{
				return -1;
			}
		}
	}
	
	return iter->second;
}

bool TcpInode::GetInodePidMap(map<int64_t, int32_t> & mapInodePid) 
{
	mapInodePid.clear();

	char path[64];
	char buf[256];
	vector<int32_t> pids;
	if (!GetPids(pids))
	{
		return false;
	}

	for (size_t i = 0; i < pids.size(); ++i) 
	{
		sprintf(path, "/proc/%d/fd/", pids[i]);
		DIR* pDir = opendir(path);
		if (!pDir) 
		{
			continue;
		}

		int len = strlen(path);
		struct dirent* pDirent = NULL;
		while ((pDirent = readdir(pDir)) != NULL) 
		{
			strcpy(path + len, pDirent->d_name);
			
			int e = readlink(path, buf, sizeof(buf));
			buf[e] = '\0';
			if(strncmp(buf, "socket:[", 8) == 0) 
			{
				uint64_t inode = 0;
				sscanf(buf + 8, "%"PRIu64, &inode);
				mapInodePid[inode] = pids[i];
			}	
		}
		closedir(pDir);
	}
	return true;
}

#endif //__LINUX__

