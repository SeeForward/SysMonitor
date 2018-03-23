
#ifndef __AUTO_LOCK_H__
#define __AUTO_LOCK_H__

#include "lock.h"
#include "uncopyable.h"

class AutoLock: public UnCopyable
{
public:
    AutoLock(CLock &lock): m_lock(lock)
    {
        m_lock.Lock();
    }

    ~AutoLock()
    {
        m_lock.Unlock();
    }

private:
    CLock &m_lock;
};

#endif //__AUTO_LOCK_H__