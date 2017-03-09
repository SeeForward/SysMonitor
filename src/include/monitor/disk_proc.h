#ifndef __DISK_PROC_H__
#define __DISK_PROC_H__

#include "type_def.h"
#include "system_time.h"

#include <map>
#include <string>
#include <vector>

class DiskProcUsage 
{
public:
	DiskProcUsage() 
	{
		m_pid = -1;
		m_read = 0;
		m_write = 0;
	}

public:
	int32_t     m_pid;
	std::string m_name;
	uint32_t    m_read;		//(byte/s)
	uint32_t    m_write;		//(byte/s)
};

class DiskProcIo 
{
public:
	DiskProcIo() 
	{
		m_pid = -1;
		m_read = 0;
		m_write = 0;
	}

public:
	int32_t  m_pid;
	uint64_t m_read;	//(byte)
	uint64_t m_write;	//(byte)
};

class DiskProc
{
public:
	DiskProc();

	//get the read write state of process on the disk
	bool GetUsages(std::vector<DiskProcUsage> &vecPdu);

private:
	bool GetProcIo(std::vector<DiskProcIo> &vecPic);

	//clear the DiskProcIoCount of process which was not live in the map 
	void ClearNotLiveProcess(std::vector<int32_t>& vecPid);

private:
	time_t m_lastClearTime;

	SystemTime m_lastProcIoTime;
	std::map<int32_t, DiskProcIo> m_mapPidDiskIo;
};

#endif //__DISK_PROC_H__

