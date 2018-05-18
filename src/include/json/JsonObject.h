#ifndef __JSON_OBJECT_H__
#define __JSON_OBJECT_H__

#include "json.h"
#include <string>

class JsonObject
{
public:
    JsonObject() {} 

    virtual ~JsonObject() {}

    virtual bool ToJson(Json::Value &jv) const = 0;

    virtual bool FromJson(const Json::Value &jv) = 0 ;

    virtual bool ToStr(std::string &str) const;

    virtual bool FromStr(const std::string &str);
};

bool JvToBool(const Json::Value &v, bool &b);
bool JvToStr(const Json::Value &jv, std::string &str);

bool JvToUInt32(const Json::Value &v, uint32_t &u);
bool JvToInt32(const Json::Value &v, int32_t &i);
bool JvToUInt64(const Json::Value &v, uint64_t &u);
bool JvToInt64(const Json::Value &v, int64_t &u);

bool JsonToStr(const Json::Value &jv, std::string &str);
bool StrToJson(const std::string& str, Json::Value &jv);

#endif //__JSON_OBJECT_H__