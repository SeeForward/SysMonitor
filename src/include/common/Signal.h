#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#include <signal.h>

typedef void (*SIGNAL_FUNC)(int);

class Signal
{
public:
    Signal(SIGNAL_FUNC signal);
    ~Signal();

    bool Action() const;

private:
    struct sigaction m_signal;
};

#endif //__SIGNAL_H__
