#ifndef __UNCOPYABLE_H__
#define __UNCOPYABLE_H__


class UnCopyable
{
protected:
	UnCopyable() {}
	
	virtual ~UnCopyable() {}

private:
	const UnCopyable& operator=(const UnCopyable&);
	
	UnCopyable(const UnCopyable&);
};

#endif //__UNCOPYABLE_H__
