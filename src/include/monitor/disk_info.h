#ifndef __DISK_INFO_H__
#define __DISK_INFO_H__

#include "type_def.h"
#include <string>
#include <vector>

//logical disk's  information
class LogiDiskInfo 
{
public:
	LogiDiskInfo() 
	{
		m_total = 0;
		m_avail = 0;
	}

public:
	std::string m_name;
	float       m_usage;		//usage rate(%)
	uint64_t    m_total;		//total size(byte)
	uint64_t    m_avail;		//availible size(byte)
};

//physical disk's information
class PhysDiskInfo 
{
public:
	PhysDiskInfo() 
	{
		m_total = 0;
	}

public:
	std::string m_name;
	std::string m_model;
	uint64_t    m_total;	//total size(byte)
};


class DiskInfo
{
public:
	//get information of all logical disks
	bool GetLogiDiskInfos(std::vector<LogiDiskInfo> &vecLdi);

	//get information of all physical disks
	bool GetPhysDiskInfos(std::vector<PhysDiskInfo> &vecPdi);

private:
#ifdef __LINUX__
	bool GetPhysDiskName(std::vector<std::string>& vecPdn);

	std::string GetPhysDiskModel(const std::string& name);

	uint64_t GetPhysDiskSize(const std::string& name);

#endif //__LINUX__
};

#endif //__DISK_INFO_H__

