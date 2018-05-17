#ifndef __REF_OBJECT_H__
#define __REF_OBJECT_H__


#include "Atomic.h"

class RefObject
{
public:
    RefObject();
    RefObject(const RefObject &other);

    virtual ~RefObject();

    const RefObject& operator=(const RefObject &other);

    int Value() const ;

protected:

    void IncRef() const;

    void DecRef() const;

    void Swap(const RefObject &other);

private:
    mutable Atomic *m_pCnt;
};


#endif //__REF_OBJECT_H__
