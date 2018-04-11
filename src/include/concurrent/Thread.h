#ifndef __THREAD_H__
#define __THREAD_H__

#include "type_def.h"
#include "IRunable.h"
#include "uncopyable.h"
#include "SmartPtr.h"

#ifdef __WINDOWS__
#   include <windows.h>
#else //__LINUX__
#   include <pthread.h>
#endif

class Thread : public UnCopyable
{
public:
    Thread(IRunable *pRunTarget, bool release = true);

    virtual ~Thread();

    int Start();

    int Join();
    bool Joinable();

    int Detch();

private:
    IRunable *m_pTarget;
    bool      m_release;

#ifndef __WINDOWS__
    pthread_t m_id;
#else
    unsigned  m_id;
    HANDLE    m_handle;
#endif
};

#endif //__THREAD_H__