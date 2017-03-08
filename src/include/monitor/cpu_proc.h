#ifndef __CPU_PROC_H__
#define __CPU_PROC_H__

#include "type_def.h"
#include "process_time.h"
#include <map>
#include <string>
#include <vector>

class CpuProcUsage 
{
public:
	CpuProcUsage() 
	{
		m_pid = 0;
		m_usage = 0;
	}

public:
	int32_t     m_pid;
	float       m_usage;     //(%)
	std::string m_name;
};

class CpuProc
{
public:
	CpuProc();
	//~CpuProc();

	//get cpu usage rate of process which was specified (%)
	float GetUsage(int32_t pid);

	//get cpu usage rate of all processes
	bool GetUsages(std::vector<CpuProcUsage> &vecPcu);

private:
	//clear the ProcessTime of not live process in tne map
	void ClearNotLiveProcess(std::vector<int32_t> vecPid);

	time_t m_lastClearTime;
	std::map<int32_t, ProcessTime> m_mapPidTime;
}; 

#endif //__CPU_PROC_H__

