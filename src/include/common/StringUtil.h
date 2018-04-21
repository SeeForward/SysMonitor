#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__

#include "type_def.h"
#include <string>
#include <vector>
#include <sstream>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <limits>
#include <cstring>
#include <vector>
#include <utility>
#include <climits>

using std::string;
using std::vector;
using std::pair;

using std::ostringstream;
using std::istringstream;
using std::setbase;
using std::numeric_limits;

using std::cout;
using std::endl;

// vector convert To string
template<typename T>
std::string ToStr(const std::vector<T> &items, char separator = ',')
{
    if (items.empty())
    {
        return "";
    }

    std::stringstream ss;
    for (size_t i = 0; i < items.size() - 1; ++i)
    {
        ss << items[i] << separator;
    }

    ss << items[items.size() - 1];

    return ss.str();
}

class StringUtil 
{
public:
    static bool IsNumber(const string& s);

    static string ToUpper(const string& s);
    static string ToLower(const string& s);

    static void Split(const string& s, char c, vector<string>& vec);
    static void Split(const string& s, const string& c, vector<string>& vec);

    static void TrimSelfLeft(string& s);
    static void TrimSelfRight(string& s);
    static void TrimSelf(string& s);

    static string TrimLeft(const string& s);
    static string TrimRight(const string& s);
    static string Trim(const string& s);

    static int32_t ToInt32(const string& s, int radix = 10);
    static int64_t ToInt64(const string& s, int radix = 10);
    static uint32_t ToUInt32(const string& s, int radix = 10);
    static uint64_t ToUInt64(const string& s, int radix = 10);
    static float ToFloat(const string& s);
    static double ToDouble(const string& s);

    template<class T>
    string ToStr(T t)
    {
        ostringstream os;
        os.precision(numeric_limits<T>::digits10);
        os << t;
        return os.str();
    }

    template<class T> 
    string ToStr(T t, int radix)
    {
        ostringstream os;
        os.precision(numeric_limits<T>::digits10);
        os << setbase(radix) << t;
        return os.str();
    }

private:
    template<typename T>
    static T& FromStr(const string& s, T& value, int radix = 10)
    {
        istringstream is(s);
        is >> setbase(radix) >> value;
        if (is.fail())
        {
            value = T();
        }
        return value;
    }
};

#endif //__STRING_UTIL_H__
