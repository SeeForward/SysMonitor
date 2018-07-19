#include "NetMessage.h"

using std::string;

NetMessage::NetMessage(int code, const string &data)
	: m_code(code), m_data(data) {}

bool NetMessage::ToJson(Json::Value &jv) const
{
    jv["code"] = m_code;
    jv["data"] = m_data;
    return true;
}

bool NetMessage::FromJson(const Json::Value &jv)
{
    return (JvToInt32(jv["code"], m_code) && JvToStr(jv["data"], m_data)); 
}


