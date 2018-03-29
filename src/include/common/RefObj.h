#ifndef __REF_OBJ_H__
#define __REF_OBJ_H__


#include <atomic>

class RefObj
{
public:
    RefObj();
    RefObj(const RefObj &other);

    virtual ~RefObj();

    const RefObj& operator=(const RefObj &other);

    inline int Value() const ;

protected:

    void IncRef() const;

    void DecRef() const;

    void Swap(const RefObj &other);

private:
    mutable std::atomic<int> *m_pCnt;
};


#endif //__REF_OBJ_H__
