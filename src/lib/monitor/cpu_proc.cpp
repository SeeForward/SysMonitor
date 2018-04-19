#include "cpu_proc.h"
#include "system_tool.h"
#include "process_name.h"

#include <set>
#include <sstream>

using std::map;
using std::set;
using std::vector;
using std::string;

string CpuProcUsage::ToStr() const
{
	std::stringstream ss;
	ss << "pid:" << m_pid << ", name:" << m_name << ", usage:" << m_usage;
	return ss.str();
}

///////////////////////////////////////////////////////////
CpuProc::CpuProc()
{
	m_lastClearTime = time(NULL);
}

float CpuProc::GetUsage(int32_t pid) 
{
	float usage = 0.0;

	ProcessTime ptCur(pid);
	if (!ptCur.Get()) 
	{
		return usage;
	}

	map<int32_t, ProcessTime>::iterator iter = m_mapPidTime.find(pid);
	if (iter != m_mapPidTime.end())
	{
		usage = CalcUsage(iter->second, ptCur);
		if(usage > 100.0)
		{
			usage = 0.0;
		}
		iter->second.m_system = ptCur.m_system;
		iter->second.m_kernel = ptCur.m_kernel;
		iter->second.m_user = ptCur.m_user;
	}
	else
	{
		usage = 0.0;
		m_mapPidTime[pid] = ptCur;
	}

	return usage;
}

bool CpuProc::GetUsages(vector<CpuProcUsage> &vecPcu) 
{
	vector<int32_t> vecPid;
	if (!GetPids(vecPid))
	{
		return false;
	}
	
	vecPcu.clear();
	vecPcu.reserve(vecPid.size());

	CpuProcUsage pcuTmp;
	for (size_t i = 0; i < vecPid.size(); ++i) 
	{
		pcuTmp.m_pid = vecPid[i];
		pcuTmp.m_name = Singleton<ProcessName>::Instance().GetNameByPid(vecPid[i]);
		pcuTmp.m_usage = GetUsage(vecPid[i]);

		if (pcuTmp.m_usage > 100.0)
		{
			pcuTmp.m_usage = 100.0;
		}
		vecPcu.push_back(pcuTmp);
	}

	time_t tmCur = time(NULL);
	if (m_lastClearTime + 60 <= tmCur) 
	{
		ClearNotLiveProcess(vecPid);
		m_lastClearTime = tmCur;
	}

	return true;
}

void CpuProc::ClearNotLiveProcess(vector<int32_t> vecPid) 
{	
	set<int32_t> setPids(vecPid.begin(), vecPid.end());

	for (map<int32_t, ProcessTime>::iterator iter = m_mapPidTime.begin(); iter != m_mapPidTime.end();)
	{
		if (setPids.find(iter->first) == setPids.end()) 
		{
			m_mapPidTime.erase(iter++);
		}
		else
		{
			++iter;
		}
	}
}

float CpuProc::CalcUsage(const ProcessTime &begin, const ProcessTime &end) 
{
	if (begin.Pid() != end.Pid())
	{
		return 0.0;	
	}
	 
	uint64_t elapseSys = end.m_system - begin.m_system;
	if (elapseSys <= 0)
	{
		return 0.0;
	}

	uint64_t elapseProc = end.m_kernel + end.m_user - begin.m_kernel - begin.m_user;
	if (elapseProc <= 0)
	{
		return 0.0;
	}

	return (float)(100 * elapseProc / (double) elapseSys);
}