#include <iostream>
#include "JsonObject.h"

using namespace std;


class JsonTest : public JsonObject
{
public:
    JsonTest(int v) : m_val(v) {}

    virtual bool ToJson(Json::Value &jv) const
    {
        jv["val"] = m_val;
        return true;;
    }

    virtual bool FromJson(const Json::Value &jv)
    {
        return JvToInt32(jv["val"], m_val);
    }

//private:
    int m_val;
};

int main(int argc, const char* argv[])
{
    Json::Value item;
    item["x"] = "12345";

    string str;
    if (!JvToStr(item["x"], str))
    {
        cerr << "json to str err" << endl;
    }
    else
    {
        cout << str << endl;
    }

    item["y"] = 12345;
    int n = 0;
    if (!JvToInt32(item["y"], n))
    {
        cerr << "json to int err" << endl;
    }
    else
    {
        cout << n << endl;
    }

    JsonTest t(1);
    Json::Value jv;

    std::string jvStr;
    if (!t.ToStr(jvStr))
    {
        cerr << " to str err" << endl;
    }

    cout << jvStr << endl;

    JsonTest t1(0);
    t1.FromStr(jvStr);
    cout << t1.m_val << endl;

    return 0;
}