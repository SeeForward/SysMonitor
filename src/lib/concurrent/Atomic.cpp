#include "Atomic.h"

Atomic::Atomic(int v)
{
    m_val = ATOMIC_INIT(v);
}

Atomic::~Atomic() {}

int Atomic::Value() const
{
    return atomic_read(&m_val);
}

void Atomic::Dec()
{
    atomic_dec(&m_val);
}

void Atomic::Inc()
{
    atomic_inc(&m_val);
}

int Atomic::DecRet()
{
    Dec();
    return Value();
}

int Atomic::IncRet()
{
    Inc();
    return Value();
}

void Atomic::Add(int v)
{
    atomic_add(v, &m_val);
}

void Atomic::Sub(int v)
{
    atomic_sub(v, &m_val);
}
