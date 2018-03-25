#ifndef __CPU_INFO_H__
#define __CPU_INFO_H__

#include "type_def.h"
#include <string>

//Processor information
class ProcessorInfo 
{
public:
	ProcessorInfo();

	//get processor's information
	bool Get();

	std::string ToStr();

private:
	void Clear(); 

public:
	bool        m_isGet;
	int32_t     m_physCount; 		//count of cpu
	int32_t     m_coreCount;		//count of physical processor
	int32_t     m_logiProcCount;		//count of logical processor(hyper-threading)
	int32_t     m_cache1Size;		//cache size(KB)
	int32_t     m_cache2Size;
	int32_t     m_cache3Size;
	float       m_rate;			//frequency(MHz)
	std::string m_model;
};

#endif //__CPU_INFO_H__

