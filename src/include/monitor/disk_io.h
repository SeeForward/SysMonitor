//Date:2017-3-6
//Author:lidengke
//Desc:query the io of disk

#ifndef __DISK_IO__
#define __DISK_IO__

#ifdef __LINUX__

#include "type_def.h"

#include <string>
#include <vector>

class DiskIo 
{
public:
	DiskIo() 
	{
		m_major = 0;
		m_minor = 0;
		m_readSectors = 0;
		m_readTicks = 0;
		m_writeSectors = 0;
		m_writeTicks = 0;
		m_ticks = 0;
		m_queueLen = 0;
	}

public:
	static bool Get(std::vector<DiskIo> &dic);

public:
	uint32_t    m_major;
	uint32_t    m_minor;	
	std::string m_name;
	uint64_t    m_readSectors;  /* sectors read */
	uint64_t    m_readTicks;    /* time in queue + service for read */
	uint64_t    m_writeSectors; /* sectors written */
	uint64_t    m_writeTicks;   /* time in queue + service for write */
	uint64_t    m_ticks;        /* time of requests in queue */
	uint64_t    m_queueLen;     /* queue length */
};

#endif //__LINUX__

#endif //__DISK_IO__

