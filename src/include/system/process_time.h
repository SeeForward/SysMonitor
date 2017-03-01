#ifndef __PROCESS_TIME_H__
#define __PROCESS_TIME_H__

#include "type_def.h"

class ProcessTime 
{
public:
	ProcessTime() 
	{
		m_pid = 0;
		m_system = 0;
		m_kernel = 0;
		m_user = 0;
	}
	
public:
	int32_t m_pid;
	uint64_t m_system;
	uint64_t m_kernel;
	uint64_t m_user;
};

//get the ProcessTime of process which was specified
bool GetProcessTime(int32_t pid, ProcessTime& pt);

//calculate cpu usage rate of Process in a period of time
float CalcProcessUsage(const ProcessTime &begin, const ProcessTime &end);


#endif //__PROCESS_TIME_H__

