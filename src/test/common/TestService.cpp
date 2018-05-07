#include <iostream>
#include "Service.h"
#include "Singleton.h"
#include <syslog.h>
#include "system_tool.h"

using namespace std;

class PrintService : public Service
{
    friend class Singleton<PrintService>;
public:
    virtual bool DoStart()
    {
        m_stop = false;
        openlog("test_service", LOG_PID, LOG_USER);
        syslog(LOG_INFO, "program started.");
        while (!m_stop)
        {
            SleepMs(1000);
            syslog(LOG_INFO, "program test.");
        }
        closelog();
        return true;
    }

	static void Stop(int)
    {
        syslog(LOG_INFO, "program end.");
        m_stop = true;
    }

private:
    PrintService() {}
    static bool m_stop;
};

bool PrintService::m_stop = true;


int main(int argc, const char * argv[])
{
    Singleton<PrintService>::Instance().Start(Signal(PrintService::Stop));

    return 0;
}