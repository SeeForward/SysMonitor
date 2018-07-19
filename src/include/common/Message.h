#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "SmartPtr.h"
#include "MessageType.h"

class Message
{
public:
    Message() : m_type(MSG_TYPE_INVALID) {}
    Message(int type) : m_type(type) {}

    virtual ~Message() {}

    int Type() const { return m_type;}

protected:
    int m_type;
};

typedef SmartPtr<Message> MessagePtr;

#endif //__MESSAGE_H__
