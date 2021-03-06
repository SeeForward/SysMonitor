#ifndef __PROCESS_NAME_H__
#define __PROCESS_NAME_H__

#include "TypeDef.h"
#include <map>
#include <string>

#include "Mutex.h"
#include "Singleton.h"

class ProcessName
{
	friend class Singleton<ProcessName>;
public:
	//get the process name by pid
	std::string GetNameByPid(int32_t pid);

	bool GetPidNameMap(std::map<int32_t, std::string> &mapPidName, bool isFlush = false);

private:
	//make only get object by Singleton
	ProcessName();

private:
	//get the mapping of pid and process name
	bool GetMap();

	bool MapEmpty();

private:
	Mutex m_mutex;
	std::map<int32_t, std::string> m_mapPidName;
};

#endif //__PROCESSOR_NAME_H__

