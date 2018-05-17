#ifndef __SMART_PTR_H__
#define __SMART_PTR_H__

#include "RefObject.h"

#include <algorithm>

template <typename T>
class SmartPtr : public RefObject
{
public:
    typedef T  ElementType;
    typedef T* PointerType;

    SmartPtr() : m_ptr(NULL) {}

    SmartPtr(T* p) : m_ptr(p) {}

    SmartPtr(const SmartPtr<T>& other) : RefObject(other), m_ptr(other.m_ptr) {}

    template <typename T1>
    SmartPtr(const SmartPtr<T1>& other) : RefObject(other), m_ptr(dynamic_cast<T>(other.m_ptr)) {}

    virtual ~SmartPtr()
    {
        if (1 == Value())
        {
            delete m_ptr;
            m_ptr = NULL;
        }
    }

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
        RefObject::Swap(other);
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
