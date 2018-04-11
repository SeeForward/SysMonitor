#ifndef __SMART_PTR_H__
#define __SMART_PTR_H__

#include "RefObj.h"

template <class T>
class SmartPtr : public RefObj
{
public:
	typedef T  ElementType;
	typedef T* PointerType;

	SmartPtr() : m_ptr(NULL) {}

	explicit SmartPtr(T* p) : m_ptr(p) {}

	SmartPtr(const SmartPtr<T>& other) : RefObj(other), m_ptr(other.m_ptr) {}

	virtual ~SmartPtr() {}

	bool isNull() const
	{
		return NULL == m_ptr;
	}

	operator bool() const
	{
		return NULL != m_ptr;
	}

	bool operator!() const
	{
		return NULL == m_ptr;
	}

	T* Get() const
	{
		return m_ptr;
	}

	T* operator->() const
	{
		return m_ptr;
	}

	T& operator*() const
	{
		return *m_ptr;
	}

	bool operator==(const SmartPtr<T>& other) const
	{
		return m_ptr == other.m_ptr;
	}

	bool operator!=(const SmartPtr<T>& other) const
	{
		return m_ptr != other.m_ptr;
	}

protected:
	void Swap(SmartPtr<T>& other)
	{
		std::swap(m_ptr, other.m_ptr);
		RefObj::Swap(other);
	}

private:
	template <class T1>
	friend class SmartPtr;

	T*  m_ptr;
};

template <class T,class U>
bool operator==(const SmartPtr<T>& lhs, const U* rhs)
{
	return lhs.Get() == rhs;
}

template <class T,class U>
bool operator==(const U* lhs, const SmartPtr<T>& rhs)
{
	return lhs == rhs.Get();
}

template <class T,class U>
bool operator!=(const SmartPtr<T>& lhs, const U* rhs)
{
	return lhs.Get() != rhs;
}

template <class T,class U>
bool operator!=(const U* lhs, const SmartPtr<T>& rhs)
{
	return lhs != rhs.Get();
}

#endif //__SMART_PTR_H__