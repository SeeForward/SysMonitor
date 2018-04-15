#ifndef __DISK_INFO_H__
#define __DISK_INFO_H__

#include "type_def.h"
#include <string>
#include <vector>

//logical disk's  information
class LogiDisk 
{
public:
	LogiDisk() 
	{
		m_usage = 0;
		m_total = 0;
		m_avail = 0;
	}

	std::string ToStr() const;

public:
	std::string m_name;
	float       m_usage;		//usage rate(%)
	uint64_t    m_total;		//total size(byte)
	uint64_t    m_avail;		//availible size(byte)
};

//physical disk's information
class PhysDisk
{
public:
	PhysDisk()
	{
		m_total = 0;
	}

	std::string ToStr() const;

public:
	std::string m_name;
	std::string m_model;
	uint64_t    m_total;	//total size(byte)
};


class DiskInfo
{
public:
	//get information of all logical disks
	static bool GetLogiDisks(std::vector<LogiDisk> &vecLd);

	//get information of all physical disks
	static bool GetPhysDisks(std::vector<PhysDisk> &vecPd);

#ifndef __WINDOWS__
	static bool GetPhysDiskName(std::vector<std::string>& vecPdn);

	static std::string GetPhysDiskModel(const std::string& name);

	static uint64_t GetPhysDiskSize(const std::string& name);

#endif //__LINUX__
};

#endif //__DISK_INFO_H__

