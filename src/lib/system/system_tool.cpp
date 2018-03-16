//Date:2016-4-14
//Author:lidengke

#include "system_tool.h"
#include "string_util.h"

#ifdef __WINDOWS__
#	include <Windows.h>
#	include <TlHelp32.h>
#	include <Psapi.h>
#else
#	include <dirent.h>
#	include <stdio.h>
#	include <string.h>
#	include <stdlib.h>
#   include <unistd.h>
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

void SleepMs(uint32_t ms)
{
#ifdef __WINDOWS__
		Sleep(ms);
#else
		usleep(ms * 1000);
#endif //__WINDOWS__
}

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

DWORD CountSetBits(ULONG_PTR bitMask)
{
	DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
	DWORD bitSetCount = 0;
	ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
	  
	for (DWORD i = 0; i <= LSHIFT; ++i) 
	{  
		bitSetCount += ((bitMask & bitTest) ? 1 : 0);
		bitTest/=2;
	}

	return bitSetCount;  
}
#endif //__WINDOWS__

