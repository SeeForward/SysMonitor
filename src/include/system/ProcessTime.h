#ifndef __PROCESS_TIME_H__
#define __PROCESS_TIME_H__

#include "TypeDef.h"
#include <string>

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

	bool Get();

	std::string ToStr() const;

public:
	uint64_t m_system;
	uint64_t m_kernel;
	uint64_t m_user;

private:
	int32_t  m_pid;
};

#endif //__PROCESS_TIME_H__

