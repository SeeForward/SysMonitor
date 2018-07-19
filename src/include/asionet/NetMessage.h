#ifndef __NET_MESSAGE_H__
#define __NET_MESSAGE_H__

#include "Message.h"
#include "SmartPtr.h"
#include "JsonObject.h"

class NetMessage : public JsonObject
{
public:
    NetMessage() {}
    NetMessage(int code, const std::string &data);

    void SetCode(int code)
    {
        m_code = code;
    }
    void SetData(const std::string &data)
    {
        m_data = data;
    }

    int Code() const
    {
        return m_code;
    }

    const std::string& Data() const 
    {
        return m_data;
    }

    virtual bool ToJson(Json::Value &jv) const;

    virtual bool FromJson(const Json::Value &jv);

private:
    int         m_code;
    std::string m_data;
};

typedef SmartPtr<NetMessage> NetMessagePtr;

#endif //__NET_MESSAGE_H__
