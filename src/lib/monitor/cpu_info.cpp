#include "cpu_info.h"

#include <iostream>
#include <sstream>

#ifdef __WINDOWS__
#	include <Windows.h>
#	include <Pdh.h>
#	include <TlHelp32.h>
#else
#	include <unistd.h>
#	include <stdio.h>
#	include <stdlib.h>
#	include <string.h>
#	include <dirent.h>
#endif //__WINDOWS__

CpuInfo::CpuInfo() 
{
	Clear();
}

void CpuInfo::Clear() 
{
	m_isGet = false;
	m_physCount = 0;
	m_coreCount = 0;
	m_logiProcCount = 0;
	m_cache1Size = 0;
	m_cache2Size = 0;
	m_cache3Size = 0;
	m_model.clear();
}

bool CpuInfo::Get() 
{
	Clear();
	m_isGet = true;
#ifdef __WINDOWS__
	m_physCount = 0;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pslpi = NULL;  
  	DWORD len = 0;  
	BOOL rc = ::GetLogicalProcessorInformation(pslpi, &len);  
	if (FALSE != rc || (::GetLastError() != ERROR_INSUFFICIENT_BUFFER)) 
	{
		return false;
	}

	pslpi = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)new char[len];
	if (NULL == pslpi) 
	{
		return false;
	}

	if(!::GetLogicalProcessorInformation(pslpi, &len)) 
	{
		return false;
	}

 	DWORD cnt = len / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = pslpi;
	for(DWORD i = 0; i < cnt; ++i)
	{
		switch (ptr[i].Relationship) 
		{
			case RelationProcessorCore:
				++m_coreCount;
				m_logiProcCount += CountSetBits(ptr[i].ProcessorMask);  
				break;
			case RelationProcessorPackage:
				++m_physCount;  
				break;
			case RelationCache:
				if(ptr[i].Cache.Level == 1) 
				{
					m_cache1Size += ptr[i].Cache.Size;
				} 
				else if(ptr[i].Cache.Level == 2) 
				{
					m_cache2Size += ptr[i].Cache.Size;
				} 
				else if(ptr[i].Cache.Level == 3) 
				{
					m_cache3Size += ptr[i].Cache.Size;
				}
				break;
			default:
				break;
		}
	}

	if(0 == m_physCount) 
	{
		m_physCount = 1;
	}

	m_cache1Size /= 1024;
	m_cache2Size /= 1024;
	m_cache3Size /= 1024;

	delete[] pslpi;

	//Open the registry
	HKEY hk;
	if (::RegCreateKey(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", &hk) != ERROR_SUCCESS) 
	{
		return false;
	}

	//Get model of CPU
	char model[100] = {0};
	DWORD modelLen = sizeof(model);
	if (::RegQueryValueEx(hk, "ProcessorNameString", NULL, NULL, (BYTE*)model, &modelLen) != ERROR_SUCCESS) 
	{
		::RegCloseKey(hk);
		return false;
	}
	m_model = model;

	//Get frequency of CPU
	DWORD rate = 0;
	DWORD rateLen = sizeof(rate);
	if (::RegQueryValueEx(hk, "~MHz", NULL, NULL, (BYTE*)&rate, &rateLen) != ERROR_SUCCESS) 
	{
		::RegCloseKey(hk);
		return false;
	}
	m_rate = (float)rate;

	//Close the registry
	::RegCloseKey(hk);
#else
	FILE *fp = fopen("/proc/cpuinfo", "r");
	if (!fp) 
	{
		return false;
	}
	char buf[1024];
	int siblings = 0;
	int cpuCores = 0;
	int lastPhysicalId = -1;
	while (fgets(buf, sizeof(buf), fp)) 
	{
		if (!strncmp(buf, "processor", 9)) 
		{
			++m_logiProcCount;
		} 
		else if (!strncmp(buf, "physical id", 11)) 
		{
			int id = -1;
			sscanf(buf, "%*s %*s %*c %d", &id);
			if(lastPhysicalId != id) 
			{
				lastPhysicalId = id;
				++m_physCount;
			}
		} 
		else if (m_model.empty() && !strncmp(buf, "model name", 10)) 
		{
			m_model = buf;
			int posBegin = m_model.find_first_of(":");
			int posEnd = m_model.find_last_not_of(" \r\n");
			m_model = m_model.substr(posBegin + 2, posEnd - (posBegin + 2));
		} 
		else if(!strncmp(buf, "cpu MHz", 7)) 
		{
			sscanf(buf, "%*s %*s %*c %f", &m_rate);
		} 
		else if (cpuCores == 0 && !strncmp(buf, "cpu cores", 9)) 
		{
			sscanf(buf, "%*s %*s %*c %d", &cpuCores);
		} 
		else if (siblings == 0 && !strncmp(buf, "siblings", 8)) 
		{
			sscanf(buf, "%*s %*c %d", &siblings);
		} 
	}
	if (0 == m_physCount) 
	{
		m_physCount = 1;
	}
	if (0 == cpuCores) 
	{
		cpuCores = 1;
	}
	if (siblings < cpuCores) 
	{
		siblings = cpuCores;
	}

	m_coreCount = m_logiProcCount / (siblings/cpuCores);

	fclose(fp);

	//L1 Cache
	fp = fopen("/sys/devices/system/cpu/cpu0/cache/index0/size", "r");
	if (!fp) 
	{
		return true;
	}
	if (!fgets(buf, sizeof(buf), fp)) 
	{
		fclose(fp);
		return true;
	}
	m_cache1Size = atoi(buf);
	fclose(fp);
	fp = fopen("/sys/devices/system/cpu/cpu0/cache/index1/size", "r");
	if (!fp) 
	{
		return true;
	}
	if (!fgets(buf, sizeof(buf), fp)) 
	{
		fclose(fp);
		return true;
	}
	m_cache1Size += atoi(buf);
	fclose(fp);
	//L2 Cache
	fp = fopen("/sys/devices/system/cpu/cpu0/cache/index2/size", "r");
	if (!fp) 
	{
		return true;
	}
	if (!fgets(buf, sizeof(buf), fp)) 
	{
		fclose(fp);
		return true;
	}
	m_cache2Size = atoi(buf);
	fclose(fp);
	//L3 Cache
	fp = fopen("/sys/devices/system/cpu/cpu0/cache/index3/size", "r");
	if (!fp) 
	{
		return true;
	}
	if (!fgets(buf, sizeof(buf), fp)) 
	{
		fclose(fp);
		return true;
	}
	m_cache3Size = atoi(buf);
	
	fclose(fp);
#endif //__WINDOWS__
	return true;
}


std::string CpuInfo::ToStr()
{
	if (!m_isGet && !Get())
	{
		return "";
	}

	std::stringstream ss;
	ss << "\tCPU Count\t\t: " << m_physCount << std::endl;
	ss << "\tPhysical Cores\t\t: " << m_coreCount << std::endl;
	ss << "\tLogical Processors\t: " << m_logiProcCount << std::endl;
	ss << "\tModel Name\t\t: " << m_model << std::endl;
	ss << "\tCPU MHz\t\t\t: " << m_rate << std::endl;
	ss << "\tL1 Cache Size\t\t: " << m_cache1Size << " KB" << std::endl;
	ss << "\tL2 Cache Size\t\t: " << m_cache2Size << " KB" << std::endl;
	ss << "\tL3 Cache Size\t\t: " << m_cache3Size << " KB" << std::endl;

	return ss.str();
}
