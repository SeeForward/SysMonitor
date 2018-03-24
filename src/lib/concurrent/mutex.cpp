#include "mutex.h"

#ifdef __WINDOWS__

Mutex::Mutex() 
{
	InitializeCriticalSection(&m_section);
}

Mutex::~Mutex()
{
	DeleteCriticalSection(&m_section);
}

void Mutex::Lock()
{
	EnterCriticalSection(&m_section);	
	assert(m_section.RecursionCount == 1);
}

bool Mutex::TryLock()
{
	if(!TryEnterCriticalSection(&m_section))
		return false;
	
	if(m_section.RecursionCount > 1)
	{
		LeaveCriticalSection(&m_section);
		return false;
	}
	return true;
}

void Mutex::Unlock()
{
	assert(m_section.RecursionCount == 1);
	LeaveCriticalSection(&m_section);
}

#else //__LINUX__

Mutex::Mutex()
{
	pthread_mutex_init(&m_mutex, NULL);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&m_mutex);
}

void Mutex::Lock()
{
	pthread_mutex_lock(&m_mutex);
}

void Mutex::Unlock()
{
	pthread_mutex_unlock(&m_mutex);
}

bool Mutex::TryLock()
{
	return pthread_mutex_trylock(&m_mutex) == 0;
}

#endif