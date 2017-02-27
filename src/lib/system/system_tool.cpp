//Date:2016-4-14
//Author:lidengke

#include "system_tool.h"
#include "string_util.h"

#ifdef __WINDOWS__
#	include <Windows.h>
#	include <TlHelp32.h>
#	include <Psapi.h>
#	pragma comment(lib, "Psapi.lib")
#else
#	include <dirent.h>
#	include <stdio.h>
#	include <string.h>
#	include <stdlib.h>
#	include <linux/major.h>
#	include <scsi/sg.h>
#	include <scsi/scsi_ioctl.h>

#	ifndef IDE_DISK_MAJOR
#	define IDE_DISK_MAJOR(M) ((M) == IDE0_MAJOR || (M) == IDE1_MAJOR || \
			   (M) == IDE2_MAJOR || (M) == IDE3_MAJOR || \
			   (M) == IDE4_MAJOR || (M) == IDE5_MAJOR || \
			   (M) == IDE6_MAJOR || (M) == IDE7_MAJOR || \
			   (M) == IDE8_MAJOR || (M) == IDE9_MAJOR)
#	endif	/* !IDE_DISK_MAJOR */

#	ifndef SCSI_DISK_MAJOR
#		ifndef SCSI_DISK8_MAJOR
#			define SCSI_DISK8_MAJOR 128
#		endif
#		ifndef SCSI_DISK15_MAJOR
#			define SCSI_DISK15_MAJOR 135
#		endif
#	define SCSI_DISK_MAJOR(M) ((M) == SCSI_DISK0_MAJOR || \
			   ((M) >= SCSI_DISK1_MAJOR && \
				(M) <= SCSI_DISK7_MAJOR) || \
			   ((M) >= SCSI_DISK8_MAJOR && \
				(M) <= SCSI_DISK15_MAJOR))
#	endif	/* !SCSI_DISK_MAJOR */
#endif //__WINDOWS__

using std::map;
using std::vector;
using std::string;

size_t GetCpuNumber() 
{
#ifdef __WINDOWS__
	SYSTEM_INFO si;
	::GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
#else
	size_t num = 0;
	FILE *fp = fopen("/proc/cpuinfo", "r");
	if (!fp) {
		return num;
	}
	char buf[1024];
	while (fgets(buf, sizeof(buf), fp)) 
	{
		if (!strncmp(buf, "processor", 9)) {
			++num;
		} 
	}

	fclose(fp);
	return num;
#endif //__WINDOWS__
}

bool GetPids(vector<int32_t> &pids) 
{
	pids.clear();	
#ifdef __WINDOWS__
	DWORD buf[2048];
	DWORD len = 0;
	if (!EnumProcesses(buf, sizeof(buf), &len)) {
        return false;
    }

    DWORD count = len / sizeof(DWORD);
    pids.reserve(count);

    for (size_t i = 0; i < count; i++)
    {
       pids.push_back(buf[i]);
    }
#else
	DIR* pDir = opendir("/proc");
	if (!pDir) {
		return false;
	}
	
	struct dirent* pCurDir = NULL;
	while ((pCurDir = readdir(pDir)) != NULL) 
	{
		//determine whether is a process
		if (IsNumber(pCurDir->d_name)) {
			pids.push_back(atoi(pCurDir->d_name));
		}
	}
	closedir(pDir);
#endif
	return true;
}

bool GetPidNameMap(map<int32_t, string> &mapPidName) 
{
	mapPidName.clear();
#ifdef __WINDOWS__
	HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapShot) {
		return false;
	}

	PROCESSENTRY32 pe = {sizeof(pe)};
	BOOL hasNext = ::Process32First(hSnapShot, &pe);
	while (hasNext) 
	{
		mapPidName[pe.th32ProcessID] = pe.szExeFile;

		hasNext = ::Process32Next(hSnapShot, &pe);
	}
	::CloseHandle(hSnapShot);
#else
	vector<int32_t> pids;
	if (!GetPids(pids))
	{
		return false;
	}

	char buf[512];
	char name[256];
	for (size_t i = 0; i < pids.size(); ++i) 
	{
		sprintf(buf, "/proc/%d/status", pids[i]);
		FILE* fp = fopen(buf, "r");
		if (!fp) {
			continue;
		}
		if (!fgets(buf, sizeof(buf), fp)) {
			continue;
		}
		sscanf(buf, "%*s %s", name);
		fclose(fp);

		mapPidName[pids[i]] = name;
	}
#endif //__WINDOWS__
	return true;
}


string GetNameByPid(int32_t pid) 
{
	static map<int32_t, string> s_mapPidName;

	//first call, load the mapping of pid and name
	if (s_mapPidName.empty())
	{
		if (!GetPidNameMap(s_mapPidName)) {
			return "";
		}
	}

	map<int32_t, string>::iterator iter = s_mapPidName.find(pid);
	//not found
	if (iter == s_mapPidName.end()) 
	{
		//reload
		if (!GetPidNameMap(s_mapPidName)) {
			return "";
		}
		iter = s_mapPidName.find(pid);
		//still not found, return ""
		if (iter == s_mapPidName.end()) {
			return "";
		}
	}
	return iter->second;
}

#ifdef __LINUX__
int IsPhysDisk(uint32_t major, uint32_t minor)
{
	if (IDE_DISK_MAJOR(major)) {
		return !(minor & 0x3F);
	} else if (SCSI_DISK_MAJOR(major)) {
		return !(minor & 0x0F);
	} else {
		return 0;
	}
}
#endif //__LINUX__

#ifdef __WINDOWS__

bool EnablePrivileges()    
{    
	HANDLE hToken;
	if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false;
	}
		
	TOKEN_PRIVILEGES tkp;
	if (!::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid)) {
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	::AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (::GetLastError() != ERROR_SUCCESS) {
		return false;
	}

	return true;    
}  

#endif //__WINDOWS__
