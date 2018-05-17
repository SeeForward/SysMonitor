#include "RefObject.h"

#include <algorithm>

RefObject::RefObject()
{
    m_pCnt = new Atomic(1);
}

RefObject::RefObject(const RefObject &other)
{
    m_pCnt = other.m_pCnt;
    IncRef();
}

RefObject::~RefObject()
{
    DecRef();
}

const RefObject& RefObject::operator=(const RefObject &other)
{
    if (this != &other)
    {
        DecRef();
        m_pCnt = other.m_pCnt;
        IncRef();
    }
    return *this;
}

int RefObject::Value() const
{
    if (m_pCnt)
    {
        return m_pCnt->Value();
    }
    return 0;
}

void RefObject::IncRef() const
{
    if (m_pCnt)
    {
        m_pCnt->Inc();
    }
}

void RefObject::DecRef() const
{
    if (NULL == m_pCnt)
    {
        return ;
    }

    if (0 == (m_pCnt->DecRet()))
    {
        delete m_pCnt;
        m_pCnt = NULL;
    }
}

void RefObject::Swap(const RefObject &other)
{
    std::swap(m_pCnt, other.m_pCnt);
}

