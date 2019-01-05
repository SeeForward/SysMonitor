#include <iostream>

#include "SystemTime.h"
#include "SystemInfo.h"
#include "SystemTool.h"
#include "CpuInfo.h"
#include "CpuProc.h"
#include "CpuUsage.h"
#include "MemoryUsage.h"
#include "MemoryProc.h"
#include "DiskInfo.h"
#include "DiskState.h"
#include "DiskProc.h"
#include "TcpConn.h"
#include "NetListen.h"
#include "AdapterIo.h"

using namespace std;

void PrintMemoryInfo()
{
	MemoryInfo mi;
	if(mi.GetInfo()) {

		cout << "\tManufacturer\t: " << mi.m_manufacturer << endl;
		cout << "\tModel\t\t: " << mi.m_model << endl;
		cout << "\tType\t\t: " << mi.m_type << endl;
		cout << "\tSingle\t\t: " << mi.m_single / 1024 / 1024 << " MB" << endl;
		cout << "\tNumber\t\t: " << mi.m_number << endl;
		cout << "\tTotal\t\t: " << mi.m_total / 1024 /1024 << " MB" << endl;
		cout << "\tSpeed\t\t: " << mi.m_speed << " MHz" << endl;
	} else {
		cout << "Call Memory::GetMemoryInfo Fail" << endl;
	}
}

int main(int argc, const char *argv[])
{
        PrintMemoryInfo();
        return 0;
}
