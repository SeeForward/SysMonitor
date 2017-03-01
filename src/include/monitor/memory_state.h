//Date:2016-3-4
//Author:lidengke
//Desc:query the information and state about physical memory and virtual memory

#ifndef __MEMORY_STATE_H__
#define __MEMORY_STATE_H__

#include "type_def.h"
#include "memory_info.h"


class MemoryState 
{
public:
	MemoryState() 
	{
		m_usage = 0.0;
		m_totalPhys = 0;
		m_availPhys = 0;
		m_totalVirt = 0;
		m_availVirt = 0;
	}

	//get memory's state
	bool GetState();

public:
	float 	m_usage;	//usage rate(%)
	uint64_t m_totalPhys;	//total physical size (byte)
	uint64_t m_availPhys;	//availible physical size (byte)
	uint64_t m_totalVirt;	//total virtual size (byte)
	uint64_t m_availVirt;	//availible virtual size (byte)
};

#endif //__MEMORY_STATE_H__
