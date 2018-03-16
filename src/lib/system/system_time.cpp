//Date:2016-3-25
//Author:lidengke
//Desc:


#include "type_def.h"
#include "system_time.h"
#include "system_tool.h"

#ifdef __WINDOWS__
#	include <Windows.h>
#else
#	include <stdio.h>
#	include <sys/param.h>
#endif //__WINDOWS__

std::string SystemTime::ToStr()
{
	std::stringstream ss;
	ss << "[system:" << m_system << ", idle:" << m_idle 
		<< ", m_kernel:" << m_kernel << ", m_user" << m_user << "]";
	return ss.str(); 
}

SystemTime SystemTime::Now() 
{
	SystemTime st;
#ifdef __WINDOWS__
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;
	if (!::GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
		return st;
	}

	ULARGE_INTEGER ui;
	ui.HighPart = idleTime.dwHighDateTime;
	ui.LowPart = idleTime.dwLowDateTime;
	st.m_idle = ui.QuadPart;

	ui.HighPart = kernelTime.dwHighDateTime;
	ui.LowPart = kernelTime.dwLowDateTime;
	st.m_kernel = ui.QuadPart;
	//kernel time include idle
	st.m_kernel -= st.m_idle;

	ui.HighPart = userTime.dwHighDateTime;
	ui.LowPart = userTime.dwLowDateTime;
	st.m_user = ui.QuadPart;

	st.m_system = st.m_idle + st.m_kernel + st.m_user;
#else
	FILE* fp = fopen("/proc/stat", "r");
	if (!fp) {
		return st;
	}

	char buf[256];
	if (!fgets(buf, sizeof(buf), fp)) {
		return st;
	}
	
	uint64_t nice = 0;
	uint64_t irq = 0;
	uint64_t iowait = 0;
	sscanf(buf, "%*s %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64, &st.m_user, &nice, &st.m_kernel, &st.m_idle, &irq, &iowait);

	st.m_system = st.m_user + nice + st.m_kernel + st.m_idle + irq + iowait;
	
	fclose(fp);
#endif //__WINDOWS__
	return st;
}

int64_t EscapeTime(const SystemTime& stBegin, const SystemTime& stEnd)
{
	size_t cpuNum = GetCpuNumber();
#ifdef __WINDOWS__
	return (stEnd.m_system - stBegin.m_system) / cpuNum / 10000;
#else
	return 1000 * (stEnd.m_system - stBegin.m_system) / HZ / cpuNum;
#endif //__WINDOWS__
}

float CalcProcessorUsage(const SystemTime &begin, const SystemTime &end) 
{
	uint64_t elapseSys = end.m_system - begin.m_system;
	if (elapseSys <= 0) {
		return 0.0;
	}

	uint64_t idle = end.m_idle - begin.m_idle;
	if (idle <= 0) {
		return 0.0;
	}

	return (float)(100 * (elapseSys - idle) / (double)elapseSys);
}

