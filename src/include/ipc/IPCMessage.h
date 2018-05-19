#ifndef __IPC_MESSAGE_H__
#define __IPC_MESSAGE_H__

#include <string>
#include "Message.h"
#include "JsonObject.h"

class IPCMessage : public Message, public JsonObject
{
public:
    IPCMessage() : Message(MSG_TYPE_IPC) {}
    IPCMessage(const std::string &cmd) : Message(MSG_TYPE_IPC), m_cmd(cmd) {}

    const std::string& Cmd() const;
    const std::string& Reply() const;

    void SetCmd(const std::string &cmd);
    void SetReply(const std::string &reply);

    virtual bool ToJson(Json::Value &jv) const;

    virtual bool FromJson(const Json::Value &jv);

protected:
    std::string m_cmd;
    std::string m_reply;
};

typedef SmartPtr<IPCMessage> IPCMsgPtr;

#endif //__IPC_MESSAGE_H__
