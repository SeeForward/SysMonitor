#ifndef __PROCESS_TIME_H__
#define __PROCESS_TIME_H__

#include "type_def.h"

class ProcessTime 
{
public:
	ProcessTime(int32_t pid = 0) 
	{
		m_pid = pid;
		m_system = 0;
		m_kernel = 0;
		m_user = 0;
	}
	
	void SetPid(int32_t pid)
	{
		m_pid = pid;
	}

	int32_t Pid() const
	{
		return m_pid;
	}

	bool GetTime();

public:
	uint64_t m_system;
	uint64_t m_kernel;
	uint64_t m_user;

private:
	int32_t  m_pid;
};

//get the ProcessTime of process which was specified
bool GetProcessTime(int32_t pid, ProcessTime& pt);

//calculate cpu usage rate of Process in a period of time
float CalcProcessUsage(const ProcessTime &begin, const ProcessTime &end);


#endif //__PROCESS_TIME_H__

