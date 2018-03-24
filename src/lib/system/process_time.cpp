#include "process_time.h"
#include "system_time.h"
#include <stdio.h>

bool ProcessTime::GetTime()
{
	if (m_pid <= 0) 
	{
		return false;
	}
#ifdef __WINDOWS__
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, m_pid);
	if (!hProcess) 
	{
		return false;
	}

	uint64_t createTime = 0;
	uint64_t exitTime = 0;
	if (!::GetProcessTimes(hProcess, (LPFILETIME)&createTime, (LPFILETIME)&exitTime, (LPFILETIME)&m_kernel, (LPFILETIME)&m_user))
	{
		::CloseHandle(hProcess);
		return false;
	}

	::CloseHandle(hProcess);
#else
	char path[32] = {0};
	sprintf(path, "/proc/%d/stat", m_pid);
	FILE* fp = fopen(path, "r");
	if (!fp)
	{
		return false;
	}

	char buf[512];
	if (!fgets(buf, sizeof(buf), fp))
	{
		fclose(fp);
		return false;
	}
	fclose(fp);

	uint64_t deadUserTime = 0;
	uint64_t deadKernelTime = 0;
	sscanf(buf, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s " 
		"%"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64, &m_user, &m_kernel, &deadUserTime, &deadKernelTime);//14,15,16,17
	m_user += deadUserTime;
	m_kernel += deadKernelTime;
#endif //__WINDOWS__

	SystemTime st = SystemTime::Now();
	m_system = st.m_system;
	return true;
}

float CalcProcessUsage(const ProcessTime &begin, const ProcessTime &end) 
{
	if (begin.Pid() != end.Pid())
	{
		return 0.0;	
	}
	 
	uint64_t elapseSys = end.m_system - begin.m_system;
	if (elapseSys <= 0)
	{
		return 0.0;
	}

	uint64_t elapseProc = end.m_kernel + end.m_user - begin.m_kernel - begin.m_user;
	if (elapseProc <= 0)
	{
		return 0.0;
	}

	return (float)(100 * elapseProc / (double) elapseSys);
}

