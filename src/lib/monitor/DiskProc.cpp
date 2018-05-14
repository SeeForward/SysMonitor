#include "DiskProc.h"
#include "SystemTool.h"
#include "ProcessName.h"

#include <stdio.h>
#include <string.h>

#include <set>

using std::set;
using std::map;
using std::string;
using std::vector;

DiskProc::DiskProc()
{
	m_lastClearTime = time(NULL);
}

bool DiskProc::GetUsages(vector<DiskProcUsage> &vecPdu) 
{
	SystemTime stCur = SystemTime::Now();
	vector<DiskProcIo> vecPic;
	GetProcIo(vecPic);
	
	vecPdu.clear();
	vecPdu.reserve(vecPic.size());

	int64_t ms = 0;
	if (m_lastProcIoTime.m_system != 0) {
		ms = SystemTime::Escape(m_lastProcIoTime, stCur);
	}
	m_lastProcIoTime = stCur;

	DiskProcUsage pduTmp;
	map<int, DiskProcIo>::iterator iter = m_mapPidDiskIo.end();
	for (size_t i = 0; i < vecPic.size(); ++i) 
	{
		pduTmp.m_pid = vecPic[i].m_pid;
		pduTmp.m_name = Singleton<ProcessName>::Instance().GetNameByPid(pduTmp.m_pid);

		if (ms) {
			//find io count of last time
			iter = m_mapPidDiskIo.find(pduTmp.m_pid);
			if (iter != m_mapPidDiskIo.end()) 
			{
				pduTmp.m_read = (uint32_t)(1000 * (vecPic[i].m_read - iter->second.m_read) / ms);
				pduTmp.m_write = (uint32_t)(1000 * (vecPic[i].m_write - iter->second.m_write) / ms);

				//record io count of this time
				iter->second.m_read = vecPic[i].m_read;
				iter->second.m_write = vecPic[i].m_write;

				vecPdu.push_back(pduTmp);
				continue;
			}
		}
		m_mapPidDiskIo[pduTmp.m_pid] = vecPic[i];

		pduTmp.m_read = 0;
		pduTmp.m_write = 0;

		vecPdu.push_back(pduTmp);
	}
	return true;
}

bool DiskProc::GetProcIo(vector<DiskProcIo> &vecPic)
{
	vector<int32_t> vecPid;
	if (!GetPids(vecPid))
	{
		return false;
	}
	
	vecPic.clear();
	vecPic.reserve(vecPid.size());

	DiskProcIo pic;
	for(size_t i = 0; i < vecPid.size(); ++i)
	{
		pic.m_pid = vecPid[i];
#ifdef __WINDOWS__
		HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, false, pic.m_pid);
		if (!hProcess) {
			continue;
		}
		IO_COUNTERS ioCounters;
		if (!::GetDiskProcIoers(hProcess, &ioCounters)) {
			continue;
		}

		pic.m_read = ioCounters.ReadTransferCount;
		pic.m_write = ioCounters.WriteTransferCount;

		::CloseHandle(hProcess);
#else //__LINUX__
		char buf[128];
		sprintf(buf, "/proc/%d/io", pic.m_pid);
		FILE* fp = fopen(buf, "r");
		if (!fp) {
			continue;
		}

		int getItemNum = 0;
		while (fgets(buf, sizeof(buf), fp)) 
		{
			if (!strncmp(buf, "read_bytes:", 11)) {
				sscanf(buf, "%*s %"PRIu64, &pic.m_read);
				++getItemNum;
			} else if (!strncmp(buf, "write_bytes:", 12)) {
				sscanf(buf, "%*s %"PRIu64, &pic.m_write);
				++getItemNum;
			}
		}
		fclose(fp);

		if (getItemNum < 2) {
			continue;
		}
#endif //__WINDOWS__
		vecPic.push_back(pic);
	}

	//clear not live processes once 60 seconds
	time_t tmCur = time(NULL);
	if (m_lastClearTime + 60 <= tmCur) {
		ClearNotLiveProcess(vecPid);
		m_lastClearTime = tmCur;
	}

	return true;
}

void DiskProc::ClearNotLiveProcess(vector<int32_t>& vecPid) 
{	
	set<int32_t> setPids(vecPid.begin(), vecPid.end());

	for (map<int32_t, DiskProcIo>::iterator iter = m_mapPidDiskIo.begin(); iter != m_mapPidDiskIo.end();)
	{
		if (setPids.find(iter->first) == setPids.end()) {
			m_mapPidDiskIo.erase(iter++);
		} else {
			++iter;
		}
	}
}

