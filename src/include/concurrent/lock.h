#ifndef _LOCK_H_
#define _LOCK_H_

class CLock
{
public:
	CLock() {}

	virtual ~CLock() {}

	virtual void Lock() = 0;

	virtual bool tryLock() = 0;

	virtual void Unlock() = 0;
};

#endif
