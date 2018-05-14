//Date:2017-3-2
//Author:lidengke
//Desc:query the state about disk

#ifndef __DISK_STATE_H__
#define __DISK_STATE_H__

#include "TypeDef.h"
#include "SystemTime.h"
#include "DiskIo.h"

#ifdef __WINDOWS__
#	include <Pdh.h>
#endif //__WINDOWS__

#include <time.h>
#include <vector>
#include <string>
#include <map>

//state of physical disk
class PhysDiskState
{
public:
	PhysDiskState() 
	{
		Clear();
	}

	void Clear() 
	{
		m_name.clear();
		m_busy = 0.0;
		m_avgQueue = 0.0;
		m_transfer = 0;
		m_read = 0;
		m_write = 0;
	}

public:
	std::string  m_name;
	float        m_busy;       //busy time(%)
	float        m_avgQueue;   //average queue length
	uint32_t     m_transfer;   //(byte/s)
	uint32_t     m_read;       //(byte/s)
	uint32_t     m_write;      //(byte/s)
};


class Disk 
{
public:
	Disk();
	~Disk();

	//get the read and write state of physical disk; total was put on the first
	bool GetStates(std::vector<PhysDiskState> &vecPds);

private:
	bool Init();
	void Destory();

#ifdef __LINUX__
private:
	SystemTime m_lastDiskIoTime;
	std::map<std::string, DiskIo> m_mapDiskIo;
#endif //__LINUX__

#ifdef __WINDOWS__
private:
	DWORD  m_counterLen;
	DWORD  m_instanceLen;
	char*  m_pCounters;
	char*  m_pInstances;
	HQUERY m_hQuery;

	std::vector<HCOUNTER>    m_vecHcounter;
	std::vector<std::string> m_vecDiskName;
#endif //__WINDOWS__
};

#endif //__DISK_STATE_H__

