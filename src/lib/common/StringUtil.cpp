#include "StringUtil.h"
#include <algorithm>

bool StringUtil::IsNumber(const string& s)
{
    string tmpStr = Trim(s);
    if (tmpStr.empty())
    {
        return false;
    }

    string str;
    if ('-' == tmpStr[0] || '+' == tmpStr[0])
    {
        str = tmpStr.substr(1, tmpStr.size() - 1);
    }
    else
    {
        str = tmpStr;
    }

    if (str.empty() || str[0] == '.'|| str[str.size() - 1] == '.')
    {
        return false;
    }

    string::size_type i = str.find_first_not_of("1234567890.");
    if (string::npos != i)
    {
        return false;
    }

    return true;
}

string StringUtil::ToUpper(const string& s)
{
    string str(s);
    transform(str.begin(), str.end(), str.begin(), toupper);
    return str;
}

string StringUtil::ToLower(const string& s)
{
    string str(s);
    transform(str.begin(), str.end(), str.begin(), tolower);
    return str;
}

void StringUtil::Split(const string& s, char c, vector<string>& vec)
{
    string str(1, c);
    Split(s, str, vec);
}

void StringUtil::Split(const string& s, const string& c, vector<string>& vec)
{
    if (s.empty() || s.size() < c.size())
    {
		vec.push_back(s);
        return ;
    }

    string::const_iterator iter = s.begin();
    do
    {
		size_t begin = iter - s.begin();
        iter = search(iter, s.end(), c.begin(), c.end());
        if (s.end() == iter)
        {
			vec.push_back(s.substr(begin, s.size() - begin));
            break;
        }
        size_t end = iter - s.begin();
		vec.push_back(s.substr(begin, end - begin));

        iter += c.size();
    } while(true);
}

void StringUtil::TrimSelfLeft(string& s)
{
    s = TrimLeft(s);
}

void StringUtil::TrimSelfRight(string& s)
{
    s = TrimRight(s);
}

void StringUtil::TrimSelf(string& s)
{
    TrimSelfLeft(s);
    TrimSelfRight(s);
}

string StringUtil::Trim(const string& s)
{
    return TrimLeft(TrimRight(s));
}

string StringUtil::TrimLeft(const string& s)
{
    string::size_type i = 0;
    for (; i < s.size(); ++i)
    {
        if (' ' != s[i])
        {
            break;
        }
    }

    string r;
    if (i < s.size())
    {
        r = s.substr(i, s.size() - i);
    }
    return r;
}

string StringUtil::TrimRight(const string& s)
{
    string::size_type i = s.size() - 1;
    for(; i != string::npos; --i)
    {
        if (' ' != s[i])
        {
            break;
        }
    }

    string r;
    if (i < s.size())
    {
        r = s.substr(0, i + 1);
    }

    return r;
}

int StringUtil::ToInt(const string& s, int radix)
{
    int i;
    FromStr(s, i, radix);
}

unsigned int StringUtil::ToUInt(const string& s, int radix)
{
    unsigned int i;
    FromStr(s, i, radix);
    return i;
}

int32_t StringUtil::ToInt32(const string& s, int radix)
{
    int32_t i;
    FromStr(s, i, radix);
    return i;
}

int64_t StringUtil::ToInt64(const string& s, int radix)
{
    int64_t i;
    FromStr(s, i, radix);
    return i;
}

uint32_t StringUtil::ToUInt32(const string& s, int radix)
{
    uint32_t u;
    FromStr(s, u, radix);
    return u;
}

uint64_t StringUtil::ToUInt64(const string& s, int radix)
{
    uint64_t u;
    FromStr(s, u, radix);
    return u;
}

float StringUtil::ToFloat(const string& s)
{
    float f;
    FromStr(s, f);
    return f;
}

double StringUtil::ToDouble(const string& s)
{
    double d;
    FromStr(s, d);
    return d;
}
