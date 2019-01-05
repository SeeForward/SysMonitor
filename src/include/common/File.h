#ifndef __FILE_H__
#define __FILE_H__

#include "TypeDef.h"

class File
{
public:
    File();
    ~File();

    int Open();
    int Flush();
    int Close(); 

    int Write(uint8_t* buf, int len);
    int Read(uint8_t* buf, int len);

    int Size();
    int Seek(int size);
    int Truncate(int size);

private:
    int m_fn;
};

#endif //__FILE_H__
