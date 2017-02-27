﻿//Date:2016-3-25
//Author:lidengke
//Desc: Time of CPU

#ifndef __SYSTEM_TIME_H__
#define __SYSTEM_TIME_H__

#include "type_def.h"

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

	//get current cpu time
	static SystemTime Now();
public:
	uint64_t m_system;

	uint64_t m_idle;
	uint64_t m_kernel;
	uint64_t m_user;
};

//calculate system escape time (millisecond)
int64_t EscapeTime(SystemTime& stBegin, SystemTime& stEnd);

#endif //__SYSTEM_TIME_H__
