//Date:2016-3-25
//Author:lidengke
//Desc: Time of CPU

#ifndef __SYSTEM_TIME_H__
#define __SYSTEM_TIME_H__

#include "type_def.h"
#include <string>
#include <sstream>

//time of cpu
class SystemTime {
public:
	//constructor
	SystemTime() {
		m_system = 0;
		m_idle = 0;
		m_kernel = 0;
		m_user = 0;
	}

	std::string ToStr() const;

	//get current cpu time
	static SystemTime Now();

	//calculate system escape time (millisecond)
	static int64_t Escape(const SystemTime& stBegin, const SystemTime& stEnd);

public:
	uint64_t m_system;

	uint64_t m_idle;
	uint64_t m_kernel;
	uint64_t m_user;
};

#endif //__SYSTEM_TIME_H__
