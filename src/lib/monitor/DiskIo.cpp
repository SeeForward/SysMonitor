#include "DiskIo.h"
#include "SystemTool.h"

#include <stdio.h>

using std::string;
using std::vector;


#ifdef __LINUX__

bool DiskIo::Get(vector<DiskIo> &vecDic) 
{
	vecDic.clear();

	FILE* fp = fopen("/proc/diskstats", "r");
	if (!fp) {
		return false;
	}

	//put the total at the front
	DiskIo dic;
	dic.m_name = "total";
	vecDic.push_back(dic);

	char name[32];
	char buf[512];
	uint32_t reads = 0;
	while (fgets(buf, sizeof(buf), fp)) 
	{
		sscanf(buf, "%4d %4d %s %u %*s %"PRIu64" %"PRIu64" %*s %*s %"PRIu64" %"PRIu64" %*u %"PRIu64" %"PRIu64,
				&dic.m_major, &dic.m_minor, name, &reads, 
				&dic.m_readSectors, &dic.m_readTicks, 
				&dic.m_writeSectors, &dic.m_writeTicks, 
				&dic.m_ticks, &dic.m_queueLen);

		if (reads && IsPhysDisk(dic.m_major, dic.m_minor)) {
				dic.m_name = name;
				vecDic.push_back(dic);
		}
	}
	fclose(fp);

	//calculate io counts of the total 
	for (size_t i = 1; i < vecDic.size(); ++i)
	{
		vecDic[0].m_readSectors += vecDic[i].m_readSectors;
		vecDic[0].m_readTicks += vecDic[i].m_readTicks;
		vecDic[0].m_writeSectors += vecDic[i].m_writeSectors;
		vecDic[0].m_writeTicks += vecDic[i].m_writeTicks;
		vecDic[0].m_ticks += vecDic[i].m_ticks;
		vecDic[0].m_queueLen += vecDic[i].m_queueLen;
	}

	return true;
}

#endif //__LINUX__


