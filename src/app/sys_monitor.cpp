// MonitorTest.cpp : Test libsysmonitor Library
//
#include "system_time.h"
#include "system_info.h"
#include "system_tool.h"
#include "cpu_info.h"
#include "cpu_proc.h"
#include "cpu_usage.h"
#include "memory_usage.h"
#include "memory_proc.h"
#include "disk_info.h"
#include "disk_state.h"
#include "disk_proc.h"
#include "tcp_conn.h"
#include "net_listen.h"
#include "adapter_io.h"

#ifdef __WINDOWS__

#	include <Windows.h>
#else
#	include <unistd.h>
#	include <stdlib.h>
#endif //__WINDOWS__

#include <signal.h>
#include <iostream>
#include <iomanip>

using namespace std;

#define SLEEP_MILLISECONDS 1000

void PrintUsage();

void PrintSystemInfo();

void TestProcessor();
void PrintProcessorInfo();
void PrintProcessorUsage();
void PrintCpuProcUsage();


void TestMemory();
void PrintMemoryInfo();
void PrintMemoryUsage();
void PrintMemoryProcUsage();


void TestDisk();
void PrintPhysDiskInfo();
void PrintLogicDiskInfo();
void PrintPhysDiskState();


void TestAdapter();

void PrintProcessDiskUsage();
void PrintAdapterInfo();
void PrintAdapterIo();
void PrintProcessTcpConn();
void PrintProcessListenProc();
string TcpStateToStr(int state);

void CleanScreen()
{
#ifdef __WINDOWS__
	system("cls");
#else
	if(-1 == system("clear")) {
		return ;
	}
#endif //__WINDOWS__
}

bool IsExsitIn(char c, const string &str);

bool isExit = false;

bool isInter = false;

void Handler(int sig);


void Signal()
{
#ifdef __WINDOWS__
	if(signal(SIGINT, Handler) == SIG_ERR) {
		cout << "signal error" << endl;
		return ;
	}
#else
    	struct sigaction act;
    	act.sa_handler = Handler;
    	sigaddset(&act.sa_mask, SIGINT);
    	act.sa_flags = SA_RESTART; 
 
		struct sigaction oldact;

    	sigaction(SIGINT, &act, &oldact);
#endif //__WINDOWS__
}

void Handler(int sig)
{
	if(sig == SIGINT) {
		isInter = true;
	}
#ifdef __WINDOWS__
	Signal();
#endif //__WINDOWS__
}


void FixSleep()
{
#ifdef __WINDOWS__
		Sleep(SLEEP_MILLISECONDS);
#else
		usleep(SLEEP_MILLISECONDS * 1000);
#endif //__WINDOWS__
}

int main(int argc, const char * argv[]) 
{
	Signal();
	char c = '\0';
	while (1) {
		if (c != 'i'){
			CleanScreen();
			PrintUsage();
		}
		cout << "Test > ";
		c = cin.get();
		cin.clear();
		cin.sync(); 
		//ignore 
		if(c == '\n') {
			c = cin.get();
			while(c == '\n') {
				cout << "Test > ";
				c = cin.get();
			}
		}
		switch(c) {
			case 'i':
				PrintSystemInfo();
				break;
			case 'p':
				TestProcessor();
				break;
			case 'm':
				TestMemory();
				break;
			case 'd':
				TestDisk();
				break;
			case 'n':
				TestAdapter();
				break;
			case 'e':
				isExit = true;
				break;
			default:
				break;
		}
		if(isExit)
			break;
	}
	return 0;
}

void PrintUsage() 
{
	cout << endl;
	cout << "This is a test for SysMonitor library." << endl;
	cout << endl;

	cout << "Usage:" << endl;

	cout << "\t i \t Print System Information" << endl;
	cout << "\t p \t Test Processor" << endl;
	cout << "\t m \t Test Memory" << endl;
	cout << "\t d \t Test Disk" << endl;
	cout << "\t n \t Test Adapter" << endl;

	cout << "\t e \t Exit Test" << endl;
}


void PrintSystemInfo()
{
	SystemInfo si;
	cout << "\tName\t: " << si.Name() << endl;
	cout << "\tDesc\t: " << si.Desc() << endl;
	cout << "\tVersion\t: " << si.Version() << endl;
	cout << "\tArch\t: " << si.Arch() << endl;
}
void TestProcessor()
{
	cout << "Processor: " << endl;
	cout << "\t i \t Print ProcessorInfo" << endl;
	cout << "\t u \t Print ProcessorUsage" << endl;
	cout << "\t p \t Print every CpuProcUsage" << endl;
	cout << "\t q \t Quit Test Processor " << endl;
	cout << "\t e \t Exit Test" << endl;
	
	char c = '\0';
	bool isQuit = false;
	while(1) {
		cout << "Processor > " ;
		c = cin.get();
		cin.sync(); 
		//ignore 
		if(c == '\n') {
			c = cin.get();
			while(c == '\n') {
				cout << "Processor > ";
				c = cin.get();
			}
		}
		switch(c) {
			case 'i':
				PrintProcessorInfo();
				break;
			case 'u':
				PrintProcessorUsage();
				break;
			case 'p': 
				PrintCpuProcUsage();
				break;
			case 'q':
				isQuit = true;
				break;
			case 'e':
				isExit = true;
				break;
		}
		cin.clear();
		if(isQuit || isExit)
			break;
		if(isInter)
			isInter = false;
	}

}

void TestMemory()
{
	cout << "Memory: " << endl;
	cout << "\t i \t Print Memory Info" << endl;
	cout << "\t u \t Print Memory Usage" << endl;
	cout << "\t p \t Print Every Process Memory Usage" << endl;
	cout << "\t q \t Quit Test Memory" << endl;
	cout << "\t e \t Exit Test" << endl;
	
	char c = '\0';
	bool isQuit = false;
	while(1) {
		cout << "Memory > ";
		c = cin.get();
		cin.clear();
		cin.sync(); 
		//ignore 
		if(c == '\n') {
			c = cin.get();
			while(c == '\n') {
				cout << "Memory > ";
				c = cin.get();
			}
		}
		switch(c) {
			case 'i':
				PrintMemoryInfo();
				break;
			case 'u':
				PrintMemoryUsage();
				break;
			case 'p': 
				PrintMemoryProcUsage();
				break;
			case 'q':
				isQuit = true;
				break;
			case 'e':
				isExit = true;
				break;
			default:
				break;
		}
		cin.clear();
		if(isQuit || isExit)
			break;
		if(isInter)
			isInter = false;
	}


}

void TestDisk()
{
	cout << "Disk: " << endl;
	cout << "\t i \t Print Physical Disk Info" << endl;
	cout << "\t l \t Print Logical Disk Info" << endl;
	cout << "\t s \t Print Physical Disk State" << endl;
	cout << "\t p \t Print Every Process Disk Usage" << endl;
	cout << "\t q \t Quit Test Disk" << endl;
	cout << "\t e \t Exit Test" << endl;

	bool isQuit = false;
	char c = '\0';
	while(1) {
		cout << "Disk > " ;
		c = cin.get();
		cin.sync(); 
		//ignore 
		if(c == '\n') {
			c = cin.get();
			while(c == '\n') {
				cout << "Disk > ";
				c = cin.get();
			}
		}
		switch(c) {
			case 'i':
				PrintPhysDiskInfo();
				break;
			case 'l':
				PrintLogicDiskInfo();
				break;
			case 's':
				PrintPhysDiskState();
				break;
			case 'p':
				PrintProcessDiskUsage();
				break;
			case 'q':
				isQuit = true;
				break;
			case 'e':
				isExit = true;
				break;
		}
		if(isQuit || isExit)
			break;
		if(isInter)
			isInter = false;
	}
}

void TestAdapter()
{
	cout << "Adapter: " << endl;
	cout << "\t i \t Print Adapter Info" << endl;
	cout << "\t s \t Print Adapter State" << endl;
	cout << "\t t \t Print Process Tcp Connetions" << endl;
	cout << "\t l \t Print Process Listen State" << endl;
	cout << "\t q \t Quit Test Adapter" << endl;
	cout << "\t e \t Exit Test" << endl;

	char c = '\0';
	bool isQuit = false;
	while(1) {
		cout << "Adapter > " ;
		c = cin.get();
		cin.sync(); 
		//ignore 
		if(c == '\n') {
			c = cin.get();
			while(c == '\n') {
				cout << "Adapter > ";
				c = cin.get();
			}
		}
		switch(c) {
			case 'i':
				PrintAdapterInfo();
				break;
			case 's':
				PrintAdapterIo();
				break;
			case 't':
				PrintProcessTcpConn();
				break;
			case 'l':
				PrintProcessListenProc();
				break;
			case 'q':
				isQuit = true;
				break;
			case 'e':
				isExit = true;
				break;
		}
		if(isQuit || isExit)
			break;
		if(isInter)
			isInter = false;
	}
}

void PrintProcessorInfo() 
{
	CpuInfo ci;
	if (ci.Get()) 
	{
		cout << "\tCPU Count\t\t: " << ci.m_physCount << endl;
		cout << "\tPhysical Cores\t\t: " << ci.m_coreCount << endl;
		cout << "\tLogical Processors\t: " << ci.m_logiProcCount << endl;
		cout << "\tModel Name\t\t: " << ci.m_model << endl;
		cout << "\tCPU MHz\t\t\t: " << ci.m_rate << endl;
		cout << "\tL1 Cache Size\t\t: " << ci.m_cache1Size << " KB" << endl;
		cout << "\tL2 Cache Size\t\t: " << ci.m_cache2Size << " KB" << endl;
		cout << "\tL3 Cache Size\t\t: " << cis.m_cache3Size << " KB" << endl;
	}
	else 
	{
		cout << "Call Processor::GetProcessorInfo Fail" << endl;
	}
}

void PrintProcessorUsage()
{
	cout.setf(ios::left);
	cout.setf(ios::fixed);

	//print head line
	cout << '\t' << setw(8) << "Total" << setw(8) << "Total";
	size_t num = GetCpuNumber();
	for(size_t i = 0; i < num; ++i)
	{
		cout << setw(8) << i;
	}
	cout << endl;

	CpuUsage cu;
	vector<float> vecUsage;

	isInter = false;
	Signal();
	while (1)
	{
		cout << '\t' << setw(8) << setprecision(2) << cu.GetUsage();	
		
		if (cu.GetUsages(vecUsage)) {

			for(size_t i = 0; i < vecUsage.size(); ++i) {
				cout  << setw(8) << setprecision(2) << vecUsage[i];
			}
			cout << endl;
		} else {
			cout << "Call Processor::GetProcessorUsages Fail" << endl;
		}
		
		if(isInter) {
			isInter = false;
			break;
		}
		FixSleep();
	}

	
}

void PrintCpuProcUsage()
{
	CpuProc cp;
	vector<CpuProcUsage> vecUsage;

	cout.setf(ios::left);
	cout.setf(ios::fixed);
	
	cout << '\t' << setw(7) << "pid" << setw(30) <<  "name" << setw(7) << "usage" << endl;

	isInter = false;
	Signal();
	while (1)
	{
		if(cp.GetUsages(vecUsage)) {
			
			int n = 0;
			for(size_t i = 0; i < vecUsage.size(); ++i) {
				if(vecUsage[i].m_usage > 0) {
					++n;
					cout << '\t' << setw(7) << vecUsage[i].m_pid << setw(30) << vecUsage[i].m_name << setw(7) << setprecision(2) << vecUsage[i].m_usage << endl;
				}
			}
			if (n > 0) {
				cout << endl;
			}
		} else {
			cout << "Call Processor::GetCpuProcUsages Fail" << endl;
		}
		if(isInter) {
			isInter = false;
			break;
		}
		FixSleep();
	}
}


void PrintMemoryInfo()
{
	//��ȡ�ڴ���Ϣ
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

void PrintMemoryUsage()
{
	//��ȡ�ڴ�״̬
	MemoryUsage mu;

	cout.setf(ios::left);
	cout.setf(ios::fixed);
	cout << '\t' << setw(7) << "Usage" << setw(20) << "TotalPhysical(MB)" << setw(20) << "AvailPhysical(MB)" 
			<< setw(20) << "TotalVirtual(MB)" << setw(20) << "AvailVirtual(MB)" << endl;
	
	isInter = false;
	Signal();		
	while(1) {
		if (mu.GetUsage()) {
			cout << '\t' << setw(7) << setprecision(2) << mu.m_usage
				<< setw(20) << mu.m_totalPhys / 1024 /1024 << setw(20) << mu.m_availPhys / 1024 /1024 
				<< setw(20) << mu.m_totalVirt / 1024 /1024 << setw(20) << mu.m_availVirt / 1024 /1024 << endl;
		} else {
			cout << "Call Memory::GetMemoryState Fail" << endl;
		}
		if(isInter) {
			isInter = false;
			break;
		}
		FixSleep();
	}
}

void PrintMemoryProcUsage()
{
	MemoryProc mu;
	vector<MemoryProcUsage> vecPmu;
	if (mu.GetUsages(vecPmu)) {
		cout.setf(ios::left);
		cout.setf(ios::fixed);
		
		cout << "\t" << setw(7) << "Pid" << setw(30) << "Name"
			<< setw(15) << "PhysMemory(KB)" << setw(15) << "PhysPeak(KB)" 
			<< setw(15) << "VirtMemory(KB)" << setw(15) << "VirtPeak(KB)" << endl;

		for(size_t i = 0; i < vecPmu.size(); ++i) {
			cout << "\t" << setw(7) << vecPmu[i].m_pid << setw(30) << vecPmu[i].m_name
				<< setw(15) << vecPmu[i].m_memUsage / 1024   << setw(15) << vecPmu[i].m_peakMemUsage / 1024
				<< setw(15) << vecPmu[i].m_virtUsage / 1024  << setw(15) << vecPmu[i].m_peakVirtUsage / 1024  << endl;
		}
	} else {
		cout << "Call Memory::GetMemoryProcUsages Fail" << endl;
	}
}



void PrintLogicDiskInfo()
{
	//��ȡ�߼�������Ϣ
	DiskInfo du;
	vector<LogiDiskInfo> vecLdi;
	if(du.GetLogiDiskInfos(vecLdi)) {
	
		cout.setf(ios::left);
		cout.setf(ios::fixed);
		cout << '\t' << setw(35) << "Name" << setw(10) << "Usage(%)" << setw(15) << "Total(MB)" << setw(15) << "Avail(MB)" << endl;

		for(size_t i = 0; i < vecLdi.size(); ++i) 
		{
			if(vecLdi[i].m_total > 0) {
				cout <<  '\t' << setw(35) << vecLdi[i].m_name << setw(10) << setprecision(2)<< vecLdi[i].m_usage
					 << setw(15) << vecLdi[i].m_total/1024/1024 << setw(15) <<vecLdi[i].m_avail/1024/1024 << endl;
			}	
		}
	} else {
		cout << "Call Disk::GetLogiDiskInfos Fail" << endl;
	}
}

void PrintPhysDiskInfo()
{
	//��ȡ���������Ϣ
	DiskInfo du;
	vector<PhysDiskInfo> vecPdi;
	if (du.GetPhysDiskInfos(vecPdi)) {
	
		cout.setf(ios::left);

		cout << '\t' << setw(7) << "Name" << setw(40) << "Model" << setw(8) << "Tatol(MB)" << endl;
		for(size_t i = 0; i < vecPdi.size(); ++i) 
		{
			if(vecPdi[i].m_total > 0) {
				cout << '\t' << setw(7) << vecPdi[i].m_name << setw(40) << vecPdi[i].m_model << setw(8) << vecPdi[i].m_total/1024/1024 << endl;
			}	
		}
	} else {
		cout << "Call Disk::GetPhysDiskInfos Fail" << endl;
	}
}

void PrintPhysDiskState()
{

	//��ȡ���д���״̬
	Disk du;
	vector<PhysDiskState> vecPds;

	cout.setf(ios::left);
	cout.setf(ios::fixed);
	cout << '\t' << setw(7) << "Name" << setw(10) << "Busy(%)" << setw(10) << "Avg." 
		<< setw(10) << "Tran(B/s)" << setw(10) << "Read(B/s)" << setw(10) << "Write(B/s)" << endl;

	cout.setf(ios::left);

	isInter = false;
	Signal();
	while (1)
	{
		if(du.GetStates(vecPds)) {
			for(size_t i = 0; i < vecPds.size(); ++i){
				cout << '\t' << setw(7) << vecPds[i].m_name << setw(10) << setprecision(2) << vecPds[i].m_busy 
					<< setw(10) << setprecision(2) << vecPds[i].m_avgQueue << setw(10) << vecPds[i].m_transfer
					<< setw(10) << vecPds[i].m_read << setw(10) << vecPds[i].m_write  << endl;
			}
			cout << endl;
		} else {
			cout << "Call Disk::GetPhysDiskStates Fail" << endl;
		}
		if(isInter) {
			isInter = false;
			break;
		}
		FixSleep();
	}
}

void PrintProcessDiskUsage()
{
	//��ȡ����ʹ�ô������
	DiskProc dp;
	cout.setf(ios::left);
	cout << '\t' << setw(7) << "Pid" << setw(30) << "Name" << setw(10) << "Read(B/s)" << setw(10) << "Write(B/s)" << endl;

	isInter = false;
	Signal();
	while (1)
	{
		vector<DiskProcUsage> vecPdu;
		if (dp.GetUsages(vecPdu)) {
			int n = 0;
			for(size_t i = 0; i < vecPdu.size(); ++i) {
				if(vecPdu[i].m_read > 0 || vecPdu[i].m_write > 0) {
					++n;
					cout << '\t' << setw(7) << vecPdu[i].m_pid << setw(30) << vecPdu[i].m_name
						<< setw(10) << vecPdu[i].m_read << setw(10) << vecPdu[i].m_write << endl;
				}
			}
			if(n > 0) {
				cout << endl;
			}
		} else {
			cout << "Call Disk::GetProcessDiskUsages Fail" << endl;
		}
		if(isInter) {
			isInter = false;
			break;
		}
		FixSleep();
	}
}

void PrintAdapterInfo()
{
	//��ȡ��������Ϣ
	vector<AdapterInfo> vecAi;
	if (AdapterInfo::GetInfos(vecAi)) {

		for(size_t i = 0; i < vecAi.size(); ++i) {
			cout << "\tName\t\t: " << vecAi[i].m_name << endl;
			cout << "\tDescription\t: " << vecAi[i].m_desc << endl;
			cout << "\tMAC\t\t: " << vecAi[i].m_physAddr << endl;
			cout << "\tIP\t\t: " << vecAi[i].m_ipAddrs[0] << endl;
			cout << "\tSpeed\t\t: " << vecAi[i].m_speed << " MB" << endl;

			cout << endl;
		}
	} else {
		cout << "Call Adapter::GetAdapterInfos Fail" << endl;
	}
}
void PrintAdapterIo()
{
	cout.setf(ios::left);

	//��ȡ������״̬
	Adapter nu;
	vector<AdapterIo> vecAs;
	cout << '\t' << setw(40) << "Name" << setw(10) << "Recv(B/s)" << setw(10) << "Send(B/s)" << endl;
	
	isInter = false;
	Signal();
	while (1)
	{
		if (nu.GetIo(vecAs)) {
			for(size_t i = 0; i < vecAs.size(); ++i) {
					cout << '\t' << setw(40) << vecAs[i].m_name << setw(10) << vecAs[i].m_recv << setw(10) << vecAs[i].m_send << endl;
			}
			cout << endl;
		} else {
			cout << "Call Adapter::GetAdapterIos Fail" << endl;
		}
		if(isInter) {
			isInter = false;
			break;
		}
		FixSleep();
	}
}
void PrintProcessTcpConn()
{
	cout.setf(ios::left);

	//��ȡ���̵�Tcp�������
	
	vector<TcpConn> vecPtc;
	if (TcpConn::Get(vecPtc)) {
	
		cout << '\t' << setw(7) << "Pid" << setw(30) << "Name" << setw(16) << "Local" << setw(15) << "Port" 
			<< setw(16) << "Remote" << setw(6) << "Port" << setw(15) << "State" << endl;
		for(size_t i = 0; i < vecPtc.size(); ++i) {
			cout << '\t' << setw(7) << vecPtc[i].m_pid << setw(30) << vecPtc[i].m_name
			<< setw(16) << vecPtc[i].m_localAddr << setw(15) << vecPtc[i].m_localPort 
			<< setw(16) << vecPtc[i].m_remoteAddr << setw(6) << vecPtc[i].m_remotePort 
			<< setw(15) << TcpStateToStr(vecPtc[i].m_state) << endl;
		}
	} else {
		cout << "Call Adapter::GetProcessTcpConns Fail" << endl;
	}

}
void PrintProcessListenProc()
{	cout.setf(ios::left);

	//��ȡ���̵������˿����
	vector<ListenProc> vecPls;
	if (ListenProc::Get(vecPls)) {
	
		cout << '\t' << setw(7) << "Pid" << setw(30) << "Name" << setw(16) << "Local" << setw(10) << "Port" << setw(10) <<"Protocol" << endl;
		for(size_t i = 0; i < vecPls.size(); ++i) {
			
			cout << '\t' << setw(7) << vecPls[i].m_pid << setw(30) << vecPls[i].m_name << setw(16) << vecPls[i].m_localAddr << setw(10) << vecPls[i].m_localPort;

			if(vecPls[i].m_type == 1)
				cout << setw(10) << "TCP" << endl;
			if(vecPls[i].m_type == 2)
				cout << setw(10) << "UDP" << endl;
		}
	} else {
		cout << "Call Adapter::GetProcessListenProcs Fail" << endl;
	}
}
string TcpStateToStr(int state) 
{
	switch (state) {
		case 0:
			return "Unknow";
		case 1:
			return "Closed";
		case 2:
			return "Listen";
		case 3:
			return "SYN sent";
		case 4:
			return "SYN rcvd";
		case 5:
			return "Established";
		case 6:
			return "FIN wait1";
		case 7:
			return "FIN wait2";
		case 8:
			return "Close wait";
		case 9:
			return "Closing";
		case 10:
			return "Last ACK";
		case 11:
			return "Time wait";
		case 12:
			return "Delete TCB";
	}

	return "Unknow";

}
bool IsExsitIn(char c, const string &str) 
{
	for(size_t i = 0; i < str.size(); ++i)
	{
		if(str[i] == c)
			return true;
	}

	return false;
}
