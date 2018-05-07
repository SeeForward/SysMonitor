#include "Service.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

using std::string;


Service::Service(){}

Service::~Service() {}

bool Service::Start()
{
    if (!Fork())
    {
        return false;
    }

    return DoStart(); 
}

bool Service::Start(const Signal &stop)
{
    if (!Fork())
    {
        return false;
    }

    stop.Action();

    return DoStart();
}

bool Service::Fork()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        return false;
    }

    if (0 != pid)
    {
        exit(0);
    }

    setsid();
    chdir("/");
    umask(0);

    close(0);
    close(1);
    close(2);

    return true;
}