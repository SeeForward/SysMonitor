#include "memory_proc.h"
#include "system_tool.h"

#ifdef __WINDOWS__
#	include <Windows.h>
#	include <TlHelp32.h>
#	include <Psapi.h>
#	include <comdef.h>
#	include <WbemIdl.h>
#	include <sstream>

	using std::stringstream;
#else
#	include <unistd.h>
#	include <stdio.h>
#	include <string.h>
#	include <dirent.h>
#	include <stdlib.h>
#endif //__WINDOWS__

using std::vector;

MemoryProc::MemoryProc() 
{
#ifdef __WINDOWS__
	EnablePrivileges();
#endif //__WINDOWS__
}

MemoryProc::~MemoryProc()
{
#ifdef __WINDOWS__
	//need reduce privileges;
#endif //__WINDOWS__
}

bool MemoryProc::GetUsage(int32_t pid, MemoryProcUsage &pmu) 
{
#ifdef __WINDOWS__
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, false, pid);
	if (NULL == hProcess) {
		return false;
	}

	pmu.m_pid = pid;
	pmu.m_name = GetNameByPid(pid);

	PROCESS_MEMORY_COUNTERS pmc;
	pmc.cb = sizeof(pmc);
	if (::GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {

		pmu.m_memUsage = pmc.WorkingSetSize;
		pmu.m_peakMemUsage = pmc.PeakWorkingSetSize;

		pmu.m_virtUsage = pmc.PagefileUsage;
		pmu.m_peakVirtUsage = pmc.PeakPagefileUsage;
		::CloseHandle(hProcess);
	} else {
		::CloseHandle(hProcess);
		return false;
	}
#else
	char path[32];
	sprintf(path, "/proc/%d/status", pid);
	FILE* fp = fopen(path, "r");
	if (!fp) {
		return false;
	}
	pmu.m_pid = pid;
	
	char buf[256];
	char name[128];
	int getItemNum = 0;
	while (fgets(buf, sizeof(buf), fp))
	{
		if(strncmp(buf, "Name:", 5) == 0) {
			sscanf(buf, "%*s %s", name);
			pmu.m_name = name;
			++getItemNum;
		} else if(strncmp(buf, "VmPeak:", 7) == 0) {
			sscanf(buf, "%*s %"PRIu64, &pmu.m_peakVirtUsage);
			pmu.m_peakVirtUsage *= 1024;
			++getItemNum;
		} else if(strncmp(buf, "VmSize:", 7) == 0) {
			sscanf(buf, "%*s %"PRIu64, &pmu.m_virtUsage);
			pmu.m_virtUsage *= 1024;
			++getItemNum;
		} else if(strncmp(buf, "VmHWM:", 6) == 0) {
			sscanf(buf, "%*s %"PRIu64, &pmu.m_peakMemUsage);
			pmu.m_peakMemUsage *= 1024;
			++getItemNum;
		} else if(strncmp(buf, "VmRSS:", 6) == 0) {
			sscanf(buf, "%*s %"PRIu64, &pmu.m_memUsage);
			pmu.m_memUsage *= 1024;
			++getItemNum;
		}
	}

	fclose(fp);
	if (getItemNum < 5) {
		return false;
	}
#endif //__WINDOWS__
	return true;
}

bool MemoryProc::GetUsages(vector<MemoryProcUsage> &vecPmu) 
{
	vector<int> vecPid;
	if (!GetPids(vecPid))
	{
		return false;
	}

	vecPmu.clear();
	vecPmu.reserve(vecPid.size());
	
	MemoryProcUsage pmuTmp;
	for (size_t i = 0; i < vecPid.size(); ++i) 
	{	
		if (GetUsage(vecPid[i], pmuTmp)) {
			vecPmu.push_back(pmuTmp);
		}
	}
	return true;
}

