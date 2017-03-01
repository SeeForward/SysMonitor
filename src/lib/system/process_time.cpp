#include "process_time.h"
#include "system_time.h"
#include <stdio.h>

bool GetProcessTime(int32_t pid, ProcessTime &pt) 
{
	if(pid <= 0) {
		return false;
	}
	pt.m_pid = pid;
#ifdef __WINDOWS__
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);
	if (!hProcess) {
		return false;
	}

	uint64_t createTime = 0;
	uint64_t exitTime = 0;
	if (!::GetProcessTimes(hProcess, (LPFILETIME)&createTime, (LPFILETIME)&exitTime, (LPFILETIME)&pt.m_kernel, (LPFILETIME)&pt.m_user)) {
		::CloseHandle(hProcess);
		return false;
	}

	::CloseHandle(hProcess);
#else
	char path[32] = {0};
	sprintf(path, "/proc/%d/stat", pid);
	FILE* fp = fopen(path, "r");
	if (!fp) {
		return false;
	}

	char buf[512];
	if (!fgets(buf, sizeof(buf), fp)) {
		return false;
	}
	fclose(fp);

	uint64_t deadUserTime = 0;
	uint64_t deadKernelTime = 0;
	sscanf(buf, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s " 
		"%"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64, &pt.m_user, &pt.m_kernel, &deadUserTime, &deadKernelTime);//第14,15,16,17项
	pt.m_user += deadUserTime;
	pt.m_kernel += deadKernelTime;
#endif //__WINDOWS__

	SystemTime st = SystemTime::Now();
	pt.m_system = st.m_system;
	return true;
}

float CalcProcessUsage(const ProcessTime &begin, const ProcessTime &end) 
{
	if (begin.m_pid != end.m_pid) {
		return 0.0;	
	}
	
	uint64_t elapseSys = end.m_system - begin.m_system;
	if (elapseSys <= 0) {
		return 0.0;
	}

	uint64_t elapseProc = end.m_kernel + end.m_user - begin.m_kernel - begin.m_user;
	if (elapseProc <= 0){
		return 0.0;
	}

	return (float)(100 * elapseProc / (double) elapseSys);
}

