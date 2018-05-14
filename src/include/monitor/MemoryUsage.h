//Date:2017-3-2
//Author:lidengke
//Desc:query the usage about physical memory and virtual memory

#ifndef __MEMORY_USAGE_H__
#define __MEMORY_USAGE_H__

#include "TypeDef.h"
#include "MemoryInfo.h"


class MemoryUsage 
{
public:
	MemoryUsage() 
	{
		m_usage = 0.0;
		m_totalPhys = 0;
		m_availPhys = 0;
		m_totalVirt = 0;
		m_availVirt = 0;
	}

	//get memory's usage
	bool GetUsage();

public:
	float 	 m_usage;	//usage rate(%)
	uint64_t m_totalPhys;	//total physical size (byte)
	uint64_t m_availPhys;	//availible physical size (byte)
	uint64_t m_totalVirt;	//total virtual size (byte)
	uint64_t m_availVirt;	//availible virtual size (byte)
};

#endif //__MEMORY_USAGE_H__
