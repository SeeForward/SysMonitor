#ifndef __MUTEX_H__
#define __MUTEX_H__

#ifdef __WINDOWS__
#include <windows.h>
#else //__LINUX__
#include <pthread.h>
#endif 

#include "uncopyable.h"
#include "ILock.h"

class Mutex : public UnCopyable, public ILock
{
public:
	Mutex();

	virtual ~Mutex();

	virtual void Lock();

	virtual bool TryLock();

	virtual void Unlock();

private:
#ifdef __WINDOWS__
	CRITICAL_SECTION m_section;
#else //__LINUX__
    pthread_mutex_t m_mutex;
#endif 
};

#endif //__MUTEX_H__
