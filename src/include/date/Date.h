#ifndef __DATE_H__
#define __DATE_H__

#include <time.h>
#include <string>

#include "DateFormat.h"
#include "StringUtil.h"

#define BASE_YEAR          (1900)
#define MONTHS_PER_YEAR    (12)
#define HOURS_PER_DAY      (24)
#define MINUTES_PER_HOUR   (60)
#define SECONDS_PER_MINUTE (60)

class Date
{
public:
    Date();
    Date(time_t t);
    Date(struct tm t);

    std::string Format(const DateFormat& df = DateFormat()) const;

    int Year() const
    {
        return m_tm.tm_year + BASE_YEAR;
    }
    int Month() const
    {
        return m_tm.tm_mon + 1;
    }
    int Day() const
    {
        return m_tm.tm_mday;
    }
    int Hour() const
    {
        return m_tm.tm_hour;
    }
    int Minute() const
    {
        return m_tm.tm_min;
    }
    int Second() const
    {
        return m_tm.tm_sec;
    }

    void SetYear(int year);
    void SetMonth(int month);
    void SetDay(int day);
    void SetHour(int hour);
    void SetMinute(int minute);
    void SetSecond(int second);
    void AddYears(int years);
    void AddMonths(int months);
    void AddDays(int days);
    void AddHours(int hours);
    void AddMinutes(int minutes);
    void AddSeconds(int seconds);

    int64_t operator - (const Date& other) const;

    bool operator == (const Date& other) const;
    bool operator != (const Date& other) const;	
    bool operator < (const Date& other) const;
    bool operator <= (const Date& other) const;
    bool operator > (const Date& other) const;
    bool operator >= (const Date& other) const;

    bool IsLeapYear() const
    {
        int year = m_tm.tm_year + BASE_YEAR;
        return IsLeapYear(year);
    }

    static Date Now()
    {
        return Date();
    }

    static int64_t Escape(const Date& dateBegin, const Date& dateEnd);

    static bool IsLeapYear(int year)
    {
        return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
    }

private:
    bool IsVaildTime(struct tm* pTm);

    int SetDate(struct tm* pTm);

private:
    struct tm m_tm;

    static uint8_t m_leapMonthDays[MONTHS_PER_YEAR];
    static uint8_t m_nonLeapMonthDays[MONTHS_PER_YEAR];
};

#endif //__DATE_H__