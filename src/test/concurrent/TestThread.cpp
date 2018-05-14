#include <iostream>
#include "Thread.h"
#include "SystemTool.h"
#include "Mutex.h"

using namespace std;

int g_n = 0;

Mutex mutex;
class TestThread: public IRunable
{
public:
    int Run()
    {
        int n = 0;
        //SleepMs(1000);
        while (n < 100)
        {
            mutex.Lock();
            ++g_n;
            cout << "thread1:" << g_n << endl;
            mutex.Unlock();
            ++n;
            //SleepMs(2000);
        }
        return 0;
    }
};

class TestThread2: public IRunable
{
public:
    int Run()
    {
        int n = 0;
        while (n < 100)
        {
            mutex.Lock();
            ++g_n;
            cout << "thread2:" << g_n << endl;
            mutex.Unlock();
            ++n;
            //SleepMs(1000);
        }
        return 0;
    }
};

int main(int argc, const char * argv[])
{
    
    IRunable *pT = new TestThread();
    Thread th1(pT, false);
    IRunable *pT2 = new TestThread2();
    Thread th2(pT2, false);

    th2.Start();
    th1.Start();

    //th1.Join();
    //th2.Join();

    th1.Detch();
    th2.Detch();

    //SleepMs(1000);
    return 0;
}