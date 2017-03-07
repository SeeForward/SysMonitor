//Date:2016-3-7
//Author:lidengke

#include "memory_usage.h"
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

#include <string>

using std::string;

bool MemoryUsage::GetUsage() 
{
#ifdef __WINDOWS__
	MEMORYSTATUSEX mse;
	mse.dwLength = sizeof(mse);
	if(!::GlobalMemoryStatusEx(&mse)) {
		return false;
	}

	m_usage = (float)mse.dwMemoryLoad;
	m_totalPhys = mse.ullTotalPhys;
	m_availPhys = mse.ullAvailPhys;
	m_totalVirt = mse.ullTotalVirtual;
	m_availVirt = mse.ullAvailVirtual;
#else
	FILE* fp = fopen("/proc/meminfo", "r");
	if (!fp) {
		return false;
	}

	char buf[256];
	int getItemNum = 0;
	uint64_t buffers = 0;
	uint64_t cached = 0;
	while (fgets(buf, sizeof(buf), fp))
	{
		if(strncmp(buf, "MemTotal:", 9) == 0) {
			sscanf(buf, "%*s %"PRIu64, &m_totalPhys);
			m_totalPhys *= 1024;
			++getItemNum;
		} else if(strncmp(buf, "MemFree:", 8) == 0) {
			sscanf(buf, "%*s %"PRIu64, &m_availPhys);
			m_availPhys *= 1024;
			++getItemNum;
		} else if(strncmp(buf, "Buffers:", 8) == 0) {
			sscanf(buf, "%*s %"PRIu64, &buffers);
			m_availPhys += buffers * 1024;
		} else if(strncmp(buf, "Cached:", 7) == 0) {
			sscanf(buf, "%*s %"PRIu64, &cached);
			m_availPhys += cached * 1024;
		} else if(strncmp(buf, "VmallocTotal:", 13) == 0) {
			sscanf(buf, "%*s %"PRIu64, &m_totalVirt);
			m_totalVirt *= 1024;
			++getItemNum;
		} else if(strncmp(buf, "VmallocUsed:", 12) == 0) {
			uint64_t usedVirt = 0;
			sscanf(buf, "%*s %"PRIu64, &usedVirt);
			usedVirt *= 1024;
			m_availVirt = m_totalVirt - usedVirt;
			++getItemNum;
		}
	}
	fclose(fp);

	if (getItemNum < 4) {
		return false;
	}
	m_usage = 100 * (m_totalPhys - m_availPhys) / (double)m_totalPhys;
#endif //__WINDOWS__
	return true;
}

