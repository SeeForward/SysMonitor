//Date: 2017-3-2
//Author:lidengke
//Desc: query usage of CpuUsage


#ifndef __CPU_USAGE_H__
#define __CPU_USAGE_H__

#include "type_def.h"
#include "system_time.h"

#ifdef __WINDOWS__
#	include <Pdh.h>
#endif //__WINDOWS__

#include <time.h>	
#include <vector>

class CpuUsage 
{
public:
	CpuUsage();
	~CpuUsage();

	//get the usage rate of total processor (%)
	float GetTotal();

	//get the usage rate of every logical processores; total was put at the first
	bool GetEvery(std::vector<float> &vecUsage);

	static float Calc(const SystemTime &begin, const SystemTime &end);

private:
	bool Init();
	void Destory();
	
private:
	SystemTime m_lastSystemTime;

#ifdef __LINUX__
	//record system time of all processores last call
	std::vector<SystemTime> m_vecLastTime;
#endif //__LINUX__

#ifdef __WINDOWS__
	std::vector<HCOUNTER> m_vecHcounter;
	HQUERY m_hQuery;
#endif //__WINDOWS__
};

#endif //__CPU_USAGE_H__

