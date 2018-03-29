#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__

#include "type_def.h"
#include <string>
#include <vector>
#include <sstream>

//checks if all chars of a string are digital chars
bool IsNumber(const char* cp);

// vector convert to string
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

#endif //__STRING_UTIL_H__
