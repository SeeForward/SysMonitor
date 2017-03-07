//Date:2016-4-14
//Author:lidengke

#ifndef __SYSTEM_TOOL_H__
#define __SYSTEM_TOOL_H__

#include "type_def.h"
#include <map>
#include <vector>
#include <string>

//get the number of the logical processors
size_t GetCpuNumber();

//get all pids of process which is running 
bool GetPids(std::vector<int32_t> &pids);

#ifdef __LINUX__
//determine whether is a physical disk
int IsPhysDisk(uint32_t major, uint32_t minor);
#endif //__LINUX__

#ifdef __WINDOWS__
//impove privilege of the current process (Windows) 
bool EnablePrivileges();

//calculate the count of bit which was set 1 in ULONG_PTR
DWORD CountSetBits(ULONG_PTR bitMask);
#endif //__WINDOWS__

#endif //__SYSTEM_TOOL_H__

