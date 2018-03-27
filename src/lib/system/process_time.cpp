#include "process_time.h"
#include "system_time.h"
#include <stdio.h>

bool ProcessTime::Get()
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

std::string ProcessTime::ToStr() const
{
	std::stringstream ss;
	ss << "[pid:" << m_pid << ", system:" << m_system << ", kernel:" << m_kernel << ", user:" << m_user << "]";
	return ss.str(); 
}

