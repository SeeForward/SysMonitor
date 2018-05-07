#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <string>
#include "Signal.h"

class Service
{
public:
    virtual ~Service();

    bool Start();
    bool Start(const Signal &stop);

protected:
    Service();
    bool Fork();
    virtual bool DoStart() = 0;
};

#endif //__SERVICE_H__
