//Date:2016-3-5
//Author:lidengke

#include "cpu_usage.h"
#include "system_tool.h"

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

#include <set>
#include <map>

using std::set;
using std::map;
using std::vector;

CpuUsage::CpuUsage() 
{
#ifdef __WINDOWS__
	m_hQuery = NULL;
	EnablePrivileges();
#endif //__WINDOWS__
	Init();
}

CpuUsage::~CpuUsage() 
{
	Destory();
}

float CpuUsage::GetUsage() 
{
	SystemTime curSt = SystemTime::Now();
	
	if (m_lastSystemTime.m_system == 0) {
		m_lastSystemTime = curSt;	
		return 0.0;
	}

	float usage = CalcProcessorUsage(m_lastSystemTime, curSt);
	m_lastSystemTime = curSt;

	if (usage > 100.0) {
		return 100.0;
	}
	return usage;
}

bool CpuUsage::GetUsages(vector<float> &vecUsage) 
{
	vecUsage.clear();
#ifdef __WINDOWS__
	//put the total at the first
	vecUsage.push_back(0.0);

	PDH_STATUS pdhStatus = ::PdhCollectQueryData(m_hQuery);
	if (pdhStatus != ERROR_SUCCESS) {
		PdhCloseQuery(m_hQuery);
		return false;
	}
	DWORD dwctrType;
	PDH_FMT_COUNTERVALUE fmtValue;
	for (size_t  i = 0; i < m_vecHcounter.size(); ++i) 
	{
		pdhStatus = ::PdhGetFormattedCounterValue(m_vecHcounter[i], PDH_FMT_DOUBLE , &dwctrType , &fmtValue) ;
		if(pdhStatus == 0xC0000BC6) {
			vecUsage.push_back(0.0);
		} else {
			vecUsage.push_back((float)fmtValue.doubleValue);
		}
	}

	float tmp = vecUsage[vecUsage.size() - 1];
	vecUsage.pop_back();
	vecUsage[0] = tmp;
#else //__LINUX__
	vector<SystemTime> vecSt;
	FILE* fp = fopen("/proc/stat", "r");
	if (!fp) {
		return false;
	}
	char buf[256];
	uint64_t kernelTime = 0;
	uint64_t niceTime = 0;
	uint64_t userTime = 0;
	uint64_t irq = 0;
	uint64_t iowait = 0;
	SystemTime stTmp;
	while (fgets(buf, sizeof(buf), fp)) 
	{
		if (strncmp(buf, "cpu", 3) == 0) 
		{
			sscanf (buf, "%*s %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64" %"PRIu64, 
				&userTime, &niceTime, &kernelTime, &stTmp.m_idle, &irq, &iowait);
			stTmp.m_system = kernelTime + niceTime + userTime + stTmp.m_idle + irq + iowait;
			vecSt.push_back(stTmp);
		}
	}
	
	fclose(fp);
	
	//first call
	if (m_vecLastTime.empty()) {
		for (size_t i = 0; i < vecSt.size(); ++i) 
		{
			vecUsage.push_back(0.0);
		}
	} else {
		for (size_t i = 0; i < vecSt.size(); ++i) 
		{
			vecUsage.push_back(CalcProcessorUsage(m_vecLastTime[i], vecSt[i]));
		}
	}
	m_vecLastTime = vecSt;

#endif //__WINDOWS__
	return true;
}

bool CpuUsage::Init() 
{
#ifdef __WINDOWS__
	if (NULL == m_hQuery && m_vecHcounter.empty()) 
	{
		vector<string> vecQueryStr;
		const char * proc = "CpuUsage";
		DWORD counterLen = 0;
		DWORD instanceLen = 0;
		::PdhEnumObjectItems(0, 0, proc, 0, &counterLen, 0, &instanceLen, PERF_DETAIL_WIZARD, 0);
		char* pCounters = new char[counterLen];
		if (NULL == pCounters) {
			return false;
		}
		char* pInstances = new char[instanceLen];
		if (NULL == pInstances) {
			delete[] pCounters;
			return false;
		}
		::PdhEnumObjectItems(0, 0, proc, pCounters, &counterLen, pInstances, &instanceLen, PERF_DETAIL_WIZARD, 0);

		for (char* pTmp = pInstances; *pTmp != 0; pTmp += strlen(pTmp) + 1) 
		{
			string queryStr = "\\" + string(proc) + "(" + string(pTmp) + ")\\" + string("% CpuUsage Time");
			vecQueryStr.push_back(queryStr);
		}
		if (pCounters) {
			delete[] pCounters;
			pCounters = NULL;
		}
		if (pInstances) {
			delete[] pInstances;
			pInstances = NULL;
		}

	    PDH_STATUS pdhStatus = ::PdhOpenQuery(0, 0, &m_hQuery);
		if (pdhStatus != ERROR_SUCCESS) {
			::PdhCloseQuery(m_hQuery);
			return false;
		}

		HCOUNTER hCounterTmp;
		for(size_t i = 0; i < vecQueryStr.size(); ++i) 
		{
			pdhStatus = ::PdhAddCounter(m_hQuery , vecQueryStr[i].c_str(), 0 , &hCounterTmp) ;
			if (pdhStatus != ERROR_SUCCESS) {
				::PdhCloseQuery(m_hQuery);
				return false;
			}
			m_vecHcounter.push_back(hCounterTmp);
		}
	}
#endif //__WINDOWS__ 
	return true;
}

void CpuUsage::Destory() 
{
#ifdef __WINDOWS__
	for (size_t  i = 0; i < m_vecHcounter.size(); ++i) 
	{
		::PdhRemoveCounter(m_vecHcounter[i]);
	}
	m_vecHcounter.clear();

	if (m_hQuery) {
		::PdhCloseQuery(m_hQuery);
		m_hQuery = NULL;
	}
#endif //__WINDOWS__
}

