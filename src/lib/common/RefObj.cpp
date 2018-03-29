#include "RefObj.h"

#include <algorithm>

RefObj::RefObj()
{
    m_pCnt = new std::atomic<int>(1);
}

RefObj::RefObj(const RefObj &other)
{
    DecRef();
    m_pCnt = other.m_pCnt;
    IncRef();
}

RefObj::~RefObj()
{
    DecRef();
}

const RefObj& RefObj::operator=(const RefObj &other)
{
    if (this != &other)
    {
        DecRef();
        m_pCnt = other.m_pCnt;
        IncRef();
    }
    return *this;
}

int RefObj::Value() const
{
    return *m_pCnt;
}

void RefObj::IncRef() const
{
    if (m_pCnt)
    {
        ++(*m_pCnt);
    }
}

void RefObj::DecRef() const
{
    if (m_pCnt && (0 == --(*m_pCnt)))
    {
        delete m_pCnt;
        m_pCnt = NULL;
    }
}

void RefObj::Swap(const RefObj &other)
{
    std::swap(m_pCnt, other.m_pCnt);
}

