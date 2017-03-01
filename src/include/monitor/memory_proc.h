#ifndef __MEMORY_PROC_H__
#define __MEMORY_PROC_H__

#include "type_def.h"
#include <string>
#include <vector>

//memory usage of process
class MemoryProcUsage 
{
public:
	MemoryProcUsage() 
	{
		m_pid = 0;
		m_peakMemUsage = 0;
		m_memUsage = 0;
		m_peakVirtUsage = 0;
		m_virtUsage = 0;
	}

public:
	int32_t m_pid;
	std::string m_name;
	int64_t m_memUsage; 		//used size of physical memory (byte)
	int64_t m_peakMemUsage; 	//peak used size of physical memory (byte)
	int64_t m_virtUsage; 		//used size of virtual memory (byte)
	int64_t m_peakVirtUsage;	//peak used size of virtual memory (byte)
};

class MemoryProc 
{
public:
	MemoryProc();
	~MemoryProc();

	//get the using memory state of process which was specified
	bool GetUsage(int32_t pid, MemoryProcUsage &pmu);

	//get the using memory state of all processes
	bool GetUsages(std::vector<MemoryProcUsage> &vecPmu);
};

#endif //__MEMORY_PROC_H__

