//Date:2016-3-7
//Author:lidengke

#include "DiskState.h"
#include "SystemTool.h"

#ifdef __WINDOWS__
#	include <Tlhelp32.h>
#else
#	include <stdio.h>
#	include <unistd.h>
#	include <string.h>
#	include <stdlib.h>
#	include <mntent.h>
#	include <fcntl.h>
#	include <sys/param.h>
#	include <sys/vfs.h>
#	include <sys/ioctl.h>
#	include <linux/fs.h>
#	include <linux/hdreg.h>
#	include <scsi/sg.h>
#	include <scsi/scsi_ioctl.h>
#endif //__WINDOWS__

#include <sstream>

//512 bytes in a sector
#define BYTES_PER_SECTOR (512)
#define MILLISECONDS_PER_SECOND (1000)

using std::map;
using std::vector;
using std::string;
using std::stringstream;

Disk::Disk()
{
#ifdef __WINDOWS__
	m_counterLen = 0;
	m_instanceLen = 0;
	m_pCounters = NULL;
	m_pInstances = NULL;
#endif //__WINDOWS__
	Init();
}

Disk::~Disk() 
{
	Destory();
}

bool Disk::GetStates(vector<PhysDiskState> &vecPds) 
{
	vecPds.clear();
#ifdef __WINDOWS__
	//add disk or remove disk will make init again
	char *pInstances = NULL;
	DWORD instanceLen = 0;
	::PdhEnumObjectItems(0, 0, "PhysicalDisk", m_pCounters, &m_counterLen, pInstances, &instanceLen, PERF_DETAIL_WIZARD, 0);
	if (instanceLen != m_instanceLen) {
		Destory();
		Init();
	}

	vecPds.reserve(m_vecDiskName.size());
	PhysDiskState pdsTmp;
	for (size_t i = 0; i < m_vecDiskName.size(); ++i) 
	{
		pdsTmp.m_name = m_vecDiskName[i];
		vecPds.push_back(pdsTmp);
	}

	PDH_STATUS pdhStatus = ::PdhCollectQueryData(m_hQuery);
	if (pdhStatus != ERROR_SUCCESS) {
		::PdhCloseQuery(m_hQuery);
		m_hQuery = NULL;
		return false;
	}

	DWORD dwctrType = 0;
	PDH_FMT_COUNTERVALUE fmtValue;
	//get the result of query
	for (size_t  i = 0; i < m_vecHcounter.size(); ++i) 
	{
		if (0 == i % 5 || 1 == i % 5) 
		{
			pdhStatus = ::PdhGetFormattedCounterValue(m_vecHcounter[i], PDH_FMT_DOUBLE , &dwctrType , &fmtValue) ;
			if (pdhStatus == ERROR_SUCCESS) 
			{
				if (0 == i % 5) {
					vecPds[i / 5].m_busy = (float)fmtValue.doubleValue;
				} else if (1 == i % 5) {
					vecPds[i / 5].m_avgQueueLen = (float)fmtValue.doubleValue;
				}
			}
			continue;
		}
		
		pdhStatus = ::PdhGetFormattedCounterValue(m_vecHcounter[i], PDH_FMT_LONG , &dwctrType , &fmtValue);
		if (pdhStatus == ERROR_SUCCESS) 
		{
			if (2 == i % 5) {
				vecPds[i / 5].m_transfer = fmtValue.longValue;
			} else if (3 == i % 5) {	
				vecPds[i / 5].m_read = fmtValue.longValue;
			} else if (4 == i % 5) {
				vecPds[i / 5].m_write = fmtValue.longValue;
			}
		}
	}
#else //__LINUX__
	SystemTime stCur = SystemTime::Now();
	//get io count of all the physical disks
	vector<DiskIo> vecDicCur;
	if (!DiskIo::Get(vecDicCur)) {
		return false;
	}
	vecPds.reserve(vecDicCur.size());

	int64_t ms = 0;
	if (m_lastDiskIoTime.m_system != 0) {
		ms = SystemTime::Escape(m_lastDiskIoTime, stCur);
	}
	m_lastDiskIoTime = stCur;
	
	PhysDiskState pdsTmp;
	map<string, DiskIo>::iterator iter = m_mapDiskIo.end();
	for(size_t i = 0; i < vecDicCur.size(); ++i) 
	{
		if (ms) 
		{
			iter = m_mapDiskIo.find(vecDicCur[i].m_name);
			if (iter != m_mapDiskIo.end()) 
			{
				//total
				if (0 == i) {
					int64_t tms = ms * (vecDicCur.size() - 1);
					pdsTmp.m_busy = 100 * (vecDicCur[i].m_ticks - iter->second.m_ticks) / (float)tms;
				} else {
					pdsTmp.m_busy = 100 * (vecDicCur[i].m_ticks - iter->second.m_ticks) / (float)ms;
				}
				if (pdsTmp.m_busy > 100.00) {
					pdsTmp.m_busy = 100.00;
				}

				pdsTmp.m_avgQueue = (vecDicCur[i].m_queueLen - iter->second.m_queueLen) / (float)ms;

				if (pdsTmp.m_busy < 0 || pdsTmp.m_avgQueue < 0 
					|| vecDicCur[i].m_readSectors < iter->second.m_readSectors 
					|| vecDicCur[i].m_writeSectors < iter->second.m_writeSectors) {
					pdsTmp.Clear();	
				} else {
					pdsTmp.m_read = (vecDicCur[i].m_readSectors - iter->second.m_readSectors) * BYTES_PER_SECTOR * MILLISECONDS_PER_SECOND / ms;
					pdsTmp.m_write = (vecDicCur[i].m_writeSectors - iter->second.m_writeSectors) * BYTES_PER_SECTOR * MILLISECONDS_PER_SECOND / ms;
					pdsTmp.m_transfer = pdsTmp.m_read + pdsTmp.m_write;
				}

				pdsTmp.m_name = vecDicCur[i].m_name;
				
				vecPds.push_back(pdsTmp);

				//record the io count of this time
				iter->second.m_ticks = vecDicCur[i].m_ticks;
				iter->second.m_queueLen = vecDicCur[i].m_queueLen;
				iter->second.m_readSectors = vecDicCur[i].m_readSectors;
				iter->second.m_writeSectors = vecDicCur[i].m_writeSectors;

				continue;
			}
		}
		m_mapDiskIo[vecDicCur[i].m_name] = vecDicCur[i];

		pdsTmp.Clear();
		pdsTmp.m_name = vecDicCur[i].m_name;

		vecPds.push_back(pdsTmp);
	}
#endif //__WINDOWS__
	return true;
}

bool Disk::Init() 
{
#ifdef __WINDOWS__
	EnablePrivileges();

	m_counterLen = 0;
	m_instanceLen = 0;
	::PdhEnumObjectItems(NULL, NULL, "PhysicalDisk", NULL, &m_counterLen, NULL, &m_instanceLen, PERF_DETAIL_WIZARD, 0);

	if (m_pCounters || m_pInstances) {
		return false;
	}
	m_pCounters = new char[m_counterLen];
	if (!m_pCounters) {
		return false;
	}
	m_pInstances = new char[m_instanceLen];
	if (!m_pInstances) {
		delete[] m_pCounters;
		m_pCounters = NULL;
		return false;
	}
	::PdhEnumObjectItems(NULL, NULL, "PhysicalDisk", m_pCounters, &m_counterLen, m_pInstances, &m_instanceLen, PERF_DETAIL_WIZARD, 0);

	vector<string> vecInstance;
	//put the total at the front
	vecInstance.push_back("_Total");
	m_vecDiskName.push_back("total");

	char name[32];
	for (char* pTmp = m_pInstances; *pTmp != 0; pTmp += strlen(pTmp) + 1) 
	{
		if (0 == strcmp(pTmp,"_Total")) {
			continue;
		}
		//number is name
		sscanf(pTmp, "%s", name);
		m_vecDiskName.push_back(name);
		vecInstance.push_back(pTmp);
	}

	vector<string> vecQueryStr;
	for (size_t i = 0; i < vecInstance.size(); ++i) 
	{
		vecQueryStr.push_back("\\PhysicalDisk(" + vecInstance[i] + ")\\% Disk Time");
		vecQueryStr.push_back("\\PhysicalDisk(" + vecInstance[i] + ")\\Avg. Disk Queue Length");
		vecQueryStr.push_back("\\PhysicalDisk(" + vecInstance[i] + ")\\Avg. Disk Bytes/Transfer");
		vecQueryStr.push_back("\\PhysicalDisk(" + vecInstance[i] + ")\\Avg. Disk Bytes/Read");
		vecQueryStr.push_back("\\PhysicalDisk(" + vecInstance[i] + ")\\Avg. Disk Bytes/Write");
	}

	PDH_STATUS pdhStatus = ::PdhOpenQuery(0, 0, &m_hQuery);
	if (pdhStatus != ERROR_SUCCESS) {
		::PdhCloseQuery(m_hQuery);
		m_hQuery = NULL;
		return false;
	}

	for (size_t i = 0; i < vecQueryStr.size(); ++i) 
	{
		HCOUNTER hCounterTmp;
		pdhStatus = ::PdhAddCounter(m_hQuery , vecQueryStr[i].c_str(), 0 , &hCounterTmp) ;
		if (pdhStatus != ERROR_SUCCESS) {
			::PdhCloseQuery(m_hQuery);
			m_hQuery = NULL;
			return false;
		}
		m_vecHcounter.push_back(hCounterTmp);
	}
#endif //__WINDOWS__
	return true;
}

void Disk::Destory() 
{
#ifdef __WINDOWS__

	for (size_t  i = 0; i < m_vecHcounter.size(); ++i) 
	{
		::PdhRemoveCounter(m_vecHcounter[i]);
	}
	m_vecHcounter.clear();
	m_vecDiskName.clear();
	
	if (m_hQuery) {
		::PdhCloseQuery(m_hQuery);
		m_hQuery = NULL;
	}

	delete[] m_pCounters;
	m_pCounters = NULL;
	delete[] m_pInstances;
	m_pInstances = NULL;
#endif //__WINDOWS__
}

