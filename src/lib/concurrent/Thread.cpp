#include "Thread.h"
#include "IRunable.h"

#ifdef __WINDOWS__
unsigned int _stdcall RUNFUNC(void* target)
{
    return reinterpret_cast<IRunable*>(target)->Run();
}
#else
void* RUNFUNC(void* target)
{
    return (void*)(reinterpret_cast<IRunable*>(target)->Run());
}
#endif

Thread::Thread(IRunable *pRunTarget, bool release): m_pTarget(pRunTarget)
{
    m_release = release;
#ifdef __WINDOWS__
    m_handle = INVALID_HANDLE_VALUE;
#endif 
    m_id = 0;
}

Thread::~Thread()
{
    if (m_release)
    {
        delete m_pTarget;
    }
}

int Thread::Start()
{
#ifdef __WINDOWS__
    m_handle = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, RUNFUNC, m_pTarget, 0, &m_id));
    if (NVALID_HANDLE_VALUE == m_handle)
    {
        return -GetLastErrors();
    }

#else // __LINUX__
    pthread_attr_t attr;
    int rc = pthread_attr_init(&attr);
    if (rc)
    {
        return rc;
    }

    rc = pthread_create(&m_id, &attr, RUNFUNC, m_pTarget);
    if (rc) 
    {
        return rc;
    }
#endif
    return 0;
}

int Thread::Join()
{
#ifdef __WINDOWS__
    if (WaitForSingleObject(m_handle, INFINITE) != WAIT_OBJECT_0)
    {
        return -GetLastError();
    }

    return detach();
#else // __LINUX__
    void* status = 0;
    int rc = pthread_join(m_id, &status);
    if (rc)
    {
        return rc;
    }
    m_id = 0;
    return rc;
#endif 
}

bool Thread::Joinable()
{
#ifdef __WINDOWS__
    return NVALID_HANDLE_VALUE != m_hanle;
#else // __LINUX__
    return 0 != m_id;
#endif 
}

int Thread::Detch()
{
#ifdef __WINDOWS__
    if (CloseHandle(m_handle) == 0)
    {
        return -GetLastError();
    }
    m_handle = INVALID_HANDLE_VALUE;
    return 0;
#else // __LINUX__
    int rc = pthread_detach(m_id);
    if (rc)
    {
        return rc;
    }
    m_id = 0;
    return rc;
#endif 
}
