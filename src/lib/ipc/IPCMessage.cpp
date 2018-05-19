#include "IPCMessage.h"


const std::string& IPCMessage::Cmd() const
{
    return m_cmd;
}

const std::string& IPCMessage::Reply() const
{
    return m_reply;
}

void IPCMessage::SetCmd(const std::string &cmd)
{
    m_cmd = cmd;
}

void IPCMessage::SetReply(const std::string &reply)
{
    m_reply = reply;
}

bool IPCMessage::ToJson(Json::Value &jv) const
{
    jv["cmd"] = m_cmd;
    jv["reply"] = m_reply;
    return true;
}

bool IPCMessage::FromJson(const Json::Value &jv)
{
    return JvToStr(jv["val"], m_reply);
}
