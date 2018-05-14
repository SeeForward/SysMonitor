#ifndef __MEMORY_INFO_H__
#define __MEMORY_INFO_H__

#include "TypeDef.h"
#include <string>

class MemoryInfo 
{
public:
	MemoryInfo() 
	{
		m_total = 0;
		m_single = 0;
		m_number = 0;
		m_speed = 0;
	}

	//get memory information
	bool GetInfo();

public:
	std::string m_manufacturer;
	std::string m_model;
	std::string m_type;		//type(Windows: DDR2 or DDR3 maybe error)
	int64_t     m_total;		//size(byte)
	int64_t     m_single;		//size of memory chip(byte)
	int32_t     m_number;		//number of memory chip
	int32_t     m_speed;		//frequency(MHz)

private:
#ifdef __WINDOWS__
	//get memory's information by WMI
	bool GetMemoryItemsInfo(const vector<string>& vecClsMember, vector<string>& vecRet);

#endif //__WINDOWS__
};

#endif //__MEMORY_INFO_H__

