#include "RefObj.h"

#include <algorithm>

RefObj::RefObj()
{
    m_pCnt = new std::atomic<int>(1);
}

RefObj::RefObj(const RefObj &other)
{
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
    if (m_pCnt)
    {
        return m_pCnt->load();
    }
    return 0;
}

void RefObj::IncRef() const
{
    if (m_pCnt)
    {
        m_pCnt->fetch_add(1);
    }
}

void RefObj::DecRef() const
{
    if (NULL == m_pCnt)
    {
        return ;
    }

    if (0 == (m_pCnt->fetch_sub(1)))
    {
        delete m_pCnt;
        m_pCnt = NULL;
    }
}

void RefObj::Swap(const RefObj &other)
{
    std::swap(m_pCnt, other.m_pCnt);
}

