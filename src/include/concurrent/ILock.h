#ifndef _LOCK_H_
#define _LOCK_H_

class ILock
{
public:
	ILock() {}

	virtual ~ILock() {}

	virtual void Lock() = 0;

	virtual bool TryLock() = 0;

	virtual void Unlock() = 0;
};

#endif
