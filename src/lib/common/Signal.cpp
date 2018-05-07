#include "Signal.h"
#include <unistd.h>

Signal::Signal(SIGNAL_FUNC signal)
{
    m_signal.sa_handler = signal;
    m_signal.sa_flags = SA_RESETHAND | SA_NODEFER;
    sigemptyset(&m_signal.sa_mask);
    sigaddset(&m_signal.sa_mask, SIGQUIT);
}

Signal::~Signal()
{
    m_signal.sa_handler = SIG_DFL;
    m_signal.sa_flags = 0;
    sigemptyset(&m_signal.sa_mask);
    sigaction(SIGINT, &m_signal, NULL);
}

bool Signal::Action() const
{
    int rc = sigaction(SIGINT, &m_signal, NULL);
    if (0 != rc)
    {
        return false;
    }
    return true;
}