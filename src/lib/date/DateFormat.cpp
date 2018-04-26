#include <time.h>

#include "Date.h"
#include "DateFormat.h"

using std::string;
using std::stringstream;

string DateFormat::Format(const Date& date) const
{
    stringstream ss;

    size_t i = 0;
    while(i < m_format.size())
    {
        switch(GetNextKeyword(i))
        {
            case KEY_YYYY:
                ss << date.Year();
                i += 4;
                break;
            case KEY_YY:
                ss << date.Year() % 100;
                i += 2;
                break;
            case KEY_MM:
                ss << date.Month();
                i += 2;
                break;
            case KEY_DD:
                ss << date.Day();
                i += 2;
                break;
            case KEY_HH24:
                ss << date.Hour();
                i += 4;
                break;
            case KEY_HH:
                ss << (date.Hour() > 12 ? date.Hour() - 12 : date.Hour());
                i += 2;
                break;
            case KEY_MI:
                ss << date.Minute();
                i += 2;
                break;
            case KEY_SS:
                ss << date.Second();
                i += 2;
                break;
            case KEY_NONE:
                ss << m_format[i];
                ++i;
        }
    }
    return ss.str();
}

Date DateFormat::Parse(const string& str) const
{
    struct tm t;
    memset(&t, 0, sizeof(struct tm));

    size_t i = 0;
    while(i < m_format.size())
    {
        switch(GetNextKeyword(i))
        {
            case KEY_YYYY:
                t.tm_year = StringUtil::ToInt(str.substr(i, 4)) + BASE_YEAR;
                i += 4;
                break;
            case KEY_YY:
                t.tm_year = (StringUtil::ToInt(str.substr(i, 2)) + BASE_YEAR) % 100;
                i += 2;
                break;
            case KEY_MM:
                t.tm_mon = StringUtil::ToInt(str.substr(i, 2)) - 1;
                i += 2;
                break;
            case KEY_DD:
                t.tm_mday = StringUtil::ToInt(str.substr(i, 2));
                i += 2;
                break;
            case KEY_HH24:
                t.tm_hour = StringUtil::ToInt(str.substr(i, 4));
                i += 4;
                break;
            case KEY_HH:
                t.tm_hour = StringUtil::ToInt(str.substr(i, 2)) + 12;
                i += 2;
                break;
            case KEY_MI:
                t.tm_min = StringUtil::ToInt(str.substr(i, 2));
                i += 2;
                break;
            case KEY_SS:
                t.tm_sec = StringUtil::ToInt(str.substr(i, 2));
                i += 2;
            case KEY_NONE:
                ++i;
        }
    }

    return Date(t);
}

DateFormat::Keyword DateFormat::GetNextKeyword(size_t pos) const
{
    switch(m_format[pos])
    {
        //年份
        case'y':
        case'Y': 
        {
            int n = 1;
            while(pos + n < m_format.size() && ('y' == m_format[pos + n] || 'Y' == m_format[pos + n]))
            {
                ++n;
                if (4 == n)
                {
                    break;
                }
            }
            if (4 == n)
            {
                return KEY_YYYY;
            }
            else if (n >= 2)
            {
                return KEY_YY;
            }
            break;
        }
        //月份或分钟
        case 'm':
        case 'M':
            if (pos + 1 < m_format.size() && ('m' == m_format[pos + 1] || 'M' == m_format[pos + 1]))
            {
                return KEY_MM;
            }
            else if (pos + 1 < m_format.size() && ('i' == m_format[pos + 1] || 'I' == m_format[pos + 1]))
            {
                return KEY_MI;
            }
            break;
        //天数
        case 'd':
        case 'D':
            if (pos + 1 < m_format.size() && ('d' == m_format[pos + 1] || 'D' == m_format[pos + 1]))
            {
                return KEY_DD;
            }
            break;
        //小时
        case 'h':
        case 'H':
            if (pos + 1 < m_format.size() && ('h' == m_format[pos + 1]|| 'H' == m_format[pos + 1]))
            {
                if (pos + 3 < m_format.size() && '2' == m_format[pos + 2] && '4' == m_format[pos + 3])
                {
                    return KEY_HH24;
                }
                else
                {
                    return KEY_HH;
                }
            }
            break;
        //秒数
        case 's':
        case 'S':
            if (pos + 1 < m_format.size() && ('s' == m_format[pos + 1]|| 'S' == m_format[pos + 1]))
            {
                return KEY_SS;
            }
            break;

    }
    return KEY_NONE;
}