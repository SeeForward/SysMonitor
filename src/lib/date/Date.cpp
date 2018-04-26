#include "Date.h"

#include <string.h>

uint8_t Date::m_leapMonthDays[MONTHS_PER_YEAR]    = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
uint8_t Date::m_nonLeapMonthDays[MONTHS_PER_YEAR] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


Date::Date()
{
    time_t timeTmp;
    time(&timeTmp);

    memcpy(&m_tm, localtime(&timeTmp), sizeof(struct tm));
}

Date::Date(time_t t)
{
    memcpy(&m_tm, localtime(&t), sizeof(struct tm));
    SetDate(&m_tm);
}

Date::Date(struct tm t)
{
    SetDate(&t);
}

void Date::SetYear(int year)
{
    struct tm tmTmp;
    memcpy(&tmTmp, &m_tm, sizeof(struct tm));

    tmTmp.tm_year = year;

    SetDate(&tmTmp);
}

void Date::SetMonth(int month)
{
    struct tm tmTmp;
    memcpy(&tmTmp, &m_tm, sizeof(struct tm));

    tmTmp.tm_mon = month - 1;
    
    SetDate(&tmTmp);
}

void Date::SetDay(int day)
{
    struct tm tmTmp;
    memcpy(&tmTmp, &m_tm, sizeof(struct tm));

    tmTmp.tm_mday = day;
    
    SetDate(&tmTmp);
}

void Date::SetHour(int hour)
{
    struct tm tmTmp;
    memcpy(&tmTmp, &m_tm, sizeof(struct tm));

    tmTmp.tm_hour = hour;
    
    SetDate(&tmTmp);
}

void Date::SetMinute(int minute)
{
    struct tm tmTmp;
    memcpy(&tmTmp, &m_tm, sizeof(struct tm));

    tmTmp.tm_min = minute;
    
    SetDate(&tmTmp);
}

void Date::SetSecond(int second)
{
    struct tm tmTmp;
    memcpy(&tmTmp, &m_tm, sizeof(struct tm));

    tmTmp.tm_sec = second;
    
    SetDate(&tmTmp);
}

void Date::AddYears(int years)
{
    
    if (m_tm.tm_mon != 1 || m_tm.tm_mday < 28)
    {
        m_tm.tm_year += years;
        return ;
    }

    if (IsLeapYear(m_tm.tm_year + BASE_YEAR))
    {
        m_tm.tm_year += years;

        if (!IsLeapYear(m_tm.tm_year + BASE_YEAR))
        {
            m_tm.tm_mday = 28;
        }
    }
    else
    {
        m_tm.tm_year += years;

        if (IsLeapYear(m_tm.tm_year + BASE_YEAR))
        {
            m_tm.tm_mday = 29;
        }
    }
}

void Date::AddMonths(int months)
{
    bool isLastDay = false;
    if (m_tm.tm_mday >= 28)
    {
        if (IsLeapYear(m_tm.tm_year + BASE_YEAR))
        {
            isLastDay = (m_leapMonthDays[m_tm.tm_mon] == m_tm.tm_mday);
        }
        else
        {
            isLastDay = (m_nonLeapMonthDays[m_tm.tm_mon] == m_tm.tm_mday);
        }
    }

    m_tm.tm_mon += months;

    m_tm.tm_year += m_tm.tm_mon / MONTHS_PER_YEAR;
    m_tm.tm_mon %= MONTHS_PER_YEAR;
    
    if (m_tm.tm_mon < 0)
    {
        --m_tm.tm_year;
        m_tm.tm_mon += MONTHS_PER_YEAR;
    }
    
    uint8_t* monthDays = IsLeapYear(m_tm.tm_year + BASE_YEAR) ? m_leapMonthDays : m_nonLeapMonthDays;
    if (isLastDay || monthDays[m_tm.tm_mon] < m_tm.tm_mday)
    {
        m_tm.tm_mday = monthDays[m_tm.tm_mon];
    }
}

void Date::AddDays(int days)
{
    AddSeconds(days * HOURS_PER_DAY * MINUTES_PER_HOUR * SECONDS_PER_MINUTE);
}

void Date::AddHours(int hours)
{
    AddSeconds(hours * MINUTES_PER_HOUR * SECONDS_PER_MINUTE);
}

void Date::AddMinutes(int minutes)
{
    AddSeconds(minutes * SECONDS_PER_MINUTE);
}

void Date::AddSeconds(int seconds)
{
    time_t timeTmp = mktime(&m_tm);

    timeTmp += seconds;

    memcpy(&m_tm, localtime(&timeTmp), sizeof(struct tm));
}

string Date::Format(const DateFormat& df) const
{
    return df.Format(*this);
}

int64_t Date::operator - (const Date& other) const
{
    return Escape(*this, other);
}

bool Date::operator == (const Date& other) const
{
    return (m_tm.tm_year == other.m_tm.tm_year
        && m_tm.tm_mon == other.m_tm.tm_mon
        && m_tm.tm_mday == other.m_tm.tm_mday 
        && m_tm.tm_hour == other.m_tm.tm_hour 
        && m_tm.tm_min == other.m_tm.tm_min 
        && m_tm.tm_sec == other.m_tm.tm_sec);
}

bool Date::operator != (const Date& other) const
{
    return !(*this == other);
}

bool Date::operator < (const Date& other) const
{
    if (m_tm.tm_year != other.m_tm.tm_year)
    {
        return m_tm.tm_year < other.m_tm.tm_year;
    }
    else if (m_tm.tm_mon != other.m_tm.tm_mon)
    {
        return m_tm.tm_mon < other.m_tm.tm_mon;
    }
    else if (m_tm.tm_mday != other.m_tm.tm_mday)
    {
        return m_tm.tm_mday < other.m_tm.tm_mday;
    }
    else if (m_tm.tm_hour != other.m_tm.tm_hour)
    {
        return m_tm.tm_hour < other.m_tm.tm_hour;
    }
    else if (m_tm.tm_min != other.m_tm.tm_min)
    {
        return m_tm.tm_min < other.m_tm.tm_min;
    }
    return m_tm.tm_sec < other.m_tm.tm_sec;
}

bool Date::operator <= (const Date& other) const
{
    return !(*this > other);
}

bool Date::operator > (const Date& other) const
{
    if (m_tm.tm_year != other.m_tm.tm_year)
    {
        return m_tm.tm_year > other.m_tm.tm_year;
    }
    else if (m_tm.tm_mon != other.m_tm.tm_mon)
    {
        return m_tm.tm_mon > other.m_tm.tm_mon;
    }
    else if (m_tm.tm_mday != other.m_tm.tm_mday)
    {
        return m_tm.tm_mday > other.m_tm.tm_mday;
    }
    else if (m_tm.tm_hour != other.m_tm.tm_hour)
    {
        return m_tm.tm_hour > other.m_tm.tm_hour;
    }
    else if (m_tm.tm_min != other.m_tm.tm_min)
    {
        return m_tm.tm_min > other.m_tm.tm_min;
    }
    return m_tm.tm_sec > other.m_tm.tm_sec;
}

bool Date::operator >= (const Date& other) const
{
    return !(*this < other);
}

bool Date::IsVaildTime(struct tm* pTm)
{
    if (pTm->tm_mon < 0 || pTm->tm_mon > 11 ||
        pTm->tm_mday < 1 || pTm->tm_mday > 31 ||
        pTm->tm_hour < 0 || pTm->tm_hour > 23 ||
        pTm->tm_min < 0 || pTm->tm_min > 59 ||
        pTm->tm_sec < 0 || pTm->tm_sec > 59)
    {
        return false;
    }

    if (IsLeapYear(pTm->tm_year + BASE_YEAR))
    {
        if (1 == pTm->tm_mon && pTm->tm_mday > 29)
        {
            return false;
        }	
    }
    else
    {
        if (1 == pTm->tm_mon && pTm->tm_mday > 28)
        {
            return false;
        }
    }

    return true;
}

int Date::SetDate(struct tm* pTm)
{
    if (IsVaildTime(pTm))
    {
        memcpy(&m_tm, pTm, sizeof(struct tm));
        return 0;
    }
    else
    {
        return -1;
    }
}

int64_t Date::Escape(const Date& dateBegin, const Date& dateEnd)
{
    time_t timeL = mktime((struct tm*)&(dateBegin.m_tm));

    time_t timeR = mktime((struct tm*)&(dateEnd.m_tm));

    return timeL - timeR;
}