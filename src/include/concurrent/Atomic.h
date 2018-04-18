#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#include "Atomic_gcc.h"

class Atomic
{
public:
    Atomic();

    ~Atomic();

    int Value() const;

    void Dec();
    void Inc();

    int DecRet();
    int IncRet();

    void Add(int v);
    void Sub(int v);

private:
    atomic_t m_val;
};

#endif //__ATOMIC_H__
