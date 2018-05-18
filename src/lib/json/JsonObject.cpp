#include "JsonObject.h"

#include <iostream>
#include <sstream>

using std::string;
using std::stringstream;

bool JsonObject::ToStr(std::string &str) const
{
    Json::Value jv;
    if (!ToJson(jv))
    {
        return false;
    }
    if (!JsonToStr(jv, str))
    {
        return false;
    }
    return true;
}

bool JsonObject::FromStr(const std::string &str)
{
    Json::Value jv;
    if (!StrToJson(str, jv))
    {
        return false;
    }
    if (!FromJson(jv))
    {
        return false;
    }
    return true;
}

bool JvToBool(const Json::Value &jv, bool &b)
{
    if (jv.isConvertibleTo(Json::booleanValue))
    {
        b = jv.asBool();
        return true;
    }
    return false;
}

bool JvToStr(const Json::Value &jv, string& str)
{
    if (jv.isConvertibleTo(Json::stringValue))
    {
        str = jv.asString();
        return true;
    }
    return false;
}

bool JvToUInt32(const Json::Value &jv, uint32_t &u)
{
    if (jv.isConvertibleTo(Json::uintValue))
    {
        u = jv.asUInt();
        return true;
    }
    return false;
}

bool JvToInt32(const Json::Value &jv, int32_t &i)
{
    if (jv.isConvertibleTo(Json::intValue))
    {
        i = jv.asInt();
        return true;
    }
    return false;
}

bool JvToUInt64(const Json::Value &jv, uint64_t &u)
{
    string str;
    if (!JvToStr(jv, str))
    {
        return false;
    }

    uint64_t u64 = 0;
    stringstream ss(str);
    ss >> u64;
    if (!ss.eof())
    {
        return false;
    }
    return true;
}

bool JvToInt64(const Json::Value &jv, int64_t &u)
{
    string str;
    if (!JvToStr(jv, str))
    {
        return false;
    }

    int64_t u64 = 0;
    stringstream ss(str);
    ss >> u64;
    if (!ss.eof())
    {
        return false;
    }
    return true;
}

bool JsonToStr(const Json::Value &jv, string& str)
{
    str = jv.toStyledString();
    return true;
}

bool StrToJson(const string& str, Json::Value &jv)
{
    Json::Reader reader;
    try
    {
        if (!reader.parse(str, jv))
        {
            return false;
        }
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
    return true;
}
