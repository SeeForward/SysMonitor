#include "string_util.h"

#include "system_tool.h"
#include "system_time.h"
#include "system_info.h"

#include "process_name.h"
#include "process_time.h"

#include <iostream>
#include <vector>
#include <map>

#include <unistd.h>

using std::cout;
using std::endl;
using std::vector;
using std::map;

void test_system_tool();

void test_system_time();

void test_system_info();

void test_process_time();

void test_process_name();

int main(int argc, const char * argv[])
{
    test_system_tool();

    test_system_time();

    test_system_info();

    test_process_time();

    test_process_name();

    return 0;
}

void test_system_tool()
{

    cout << "cpu number:" << GetCpuNumber() << endl;

    vector<int32_t> pids;
    bool ret = GetPids(pids);
    if (ret)
    {

        cout << "pids:[" << ToStr(pids) << "]" << endl;
    }
    else
    {
        cout << "get pids err" << endl;
    }
}

void test_system_time()
{
    SystemTime stBegin = SystemTime::Now();
    cout << stBegin.ToStr() << endl;

    SleepMs(1000);

    SystemTime stEnd = SystemTime::Now();
    cout << stEnd.ToStr() << endl;

    int64_t eT = EscapeTime(stBegin, stEnd);

    cout << "escape time: " << eT << endl; 

    float cpuUsage = CalcProcessorUsage(stBegin, stEnd); 
    cout << "cpu usage: " << cpuUsage << "%" << endl;
}

void test_system_info()
{
    SystemInfo si;
	cout << "\tName\t: " << si.Name() << endl;
	cout << "\tDesc\t: " << si.Desc() << endl;
	cout << "\tVersion\t: " << si.Version() << endl;
	cout << "\tArch\t: " << si.Arch() << endl;
}

void test_process_time()
{
    vector<int32_t> pids;
    if (!GetPids(pids))
    {
        return ;
    }

    for (size_t i = 0; i < pids.size(); ++i)
    {
        ProcessTime pt(pids[i]);
        pt.GetTime();
        cout << pt.ToStr() << endl;
    }
}

void test_process_name()
{
    ProcessName *pPn = ProcessName::Inst();

    map<int32_t, std::string> mapPidName;
    pPn->GetPidNameMap(mapPidName);

    for (map<int32_t, string>::iterator itr = mapPidName.begin(); itr != mapPidName.end(); ++itr)
    {
        cout << itr->first << ":" << itr->second << endl;
    }
}
