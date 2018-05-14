
#ifndef __AUTO_LOCK_H__
#define __AUTO_LOCK_H__

#include "ILock.h"
#include "UnCopyable.h"

class AutoLock: public UnCopyable
{
public:
    AutoLock(ILock &lock): m_lock(lock)
    {
        m_lock.Lock();
    }

    ~AutoLock()
    {
        m_lock.Unlock();
    }

private:
    ILock &m_lock;
};

#endif //__AUTO_LOCK_H__