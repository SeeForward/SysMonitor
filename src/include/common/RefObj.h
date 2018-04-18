#ifndef __REF_OBJ_H__
#define __REF_OBJ_H__


#include "Atomic.h"

class RefObj
{
public:
    RefObj();
    RefObj(const RefObj &other);

    virtual ~RefObj();

    const RefObj& operator=(const RefObj &other);

    int Value() const ;

protected:

    void IncRef() const;

    void DecRef() const;

    void Swap(const RefObj &other);

private:
    mutable Atomic *m_pCnt;
};


#endif //__REF_OBJ_H__
