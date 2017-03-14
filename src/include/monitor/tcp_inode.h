#ifndef __TCP_INODE_H__
#define __TCP_INODE_H__

#ifdef __LINUX__

#include "type_def.h"
#include <map>

class TcpInode
{
public:
	static TcpInode* Inst();	

	int32_t GetTcpPidByInode(int64_t inode);

private:
	//make only get object by Inst()
	TcpInode();

private:

	bool GetInodePidMap(std::map<int64_t, int32_t> & mapInodePid);
private:
	//the mapping of tcp connection's inode and process's pid
	std::map<int64_t, int32_t> m_mapInodePid;

};

#endif //__LINUX__

#endif //__TCP_INODE_H__

