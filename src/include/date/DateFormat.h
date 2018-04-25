#ifndef __DATE_FORMAT_H__
#define __DATE_FORMAT_H__

#include <string>
#include <sstream>

class Date;

class DateFormat
{
public:
	DateFormat(const std::string& format = "YYYY-MM-DD HH24:MI:SS") : m_format(format) {}

	std::string Format(const Date& date) const;

	Date Parse(const std::string& s) const;

	void SetFormatStr(const std::string& format)
    {
		m_format = format;
	}

	std::string GetFormatStr() const
    {
		return m_format;
	}

private:
	enum Keyword {KEY_YYYY, KEY_YY, KEY_MM, KEY_DD, KEY_HH24, KEY_HH, KEY_MI, KEY_SS, KEY_NONE};

	Keyword GetNextKeyword(size_t pos) const;

private:
	std::string m_format;
};

#endif // __DATE_FORMAT_H__
