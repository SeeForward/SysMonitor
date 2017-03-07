#include "process_name.h"
#include "system_tool.h"

#include <stdio.h>

using std::map;
using std::string;
using std::vector;


ProcessName* ProcessName::Inst()
{
	static ProcessName pn;
	return &pn;
}

string ProcessName::GetNameByPid(int32_t pid) 
{
	//first call, load the mapping of pid and name
	if (m_mapPidName.empty())
	{
		if (!GetMap()) {
			return "";
		}
	}

	map<int32_t, string>::iterator iter = m_mapPidName.find(pid);
	//not found
	if (iter == m_mapPidName.end()) 
	{
		//reload
		if (!GetMap()) {
			return "";
		}
		iter = m_mapPidName.find(pid);
		//still not found, return ""
		if (iter == m_mapPidName.end()) {
			return "";
		}
	}
	return iter->second;
}

bool ProcessName::GetMap() 
{
	m_mapPidName.clear();
#ifdef __WINDOWS__
	HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapShot) {
		return false;
	}

	PROCESSENTRY32 pe = {sizeof(pe)};
	BOOL hasNext = ::Process32First(hSnapShot, &pe);
	while (hasNext) 
	{
		m_mapPidName[pe.th32ProcessID] = pe.szExeFile;

		hasNext = ::Process32Next(hSnapShot, &pe);
	}
	::CloseHandle(hSnapShot);
#else
	vector<int32_t> pids;
	if (!GetPids(pids))
	{
		return false;
	}

	char buf[512];
	char name[256];
	for (size_t i = 0; i < pids.size(); ++i) 
	{
		sprintf(buf, "/proc/%d/status", pids[i]);
		FILE* fp = fopen(buf, "r");
		if (!fp) {
			continue;
		}
		if (!fgets(buf, sizeof(buf), fp)) {
			continue;
		}
		sscanf(buf, "%*s %s", name);
		fclose(fp);

		m_mapPidName[pids[i]] = name;
	}
#endif //__WINDOWS__
	return true;
}

