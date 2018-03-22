#ifndef _IRUNABLE_H_
#define _IRUNABLE_H_

class IRunable 
{
public:
	IRunable() {}

	virtual ~IRunable() {}

	virtual void Run() = 0;
};

#endif //_IRUNABLE_H_
