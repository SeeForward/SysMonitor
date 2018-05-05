#include "Appender.h"
#include "Date.h"

#include <iostream>

using std::string;
using std::cout;

Appender::Appender(const string& name, const DateFormat &dateFormat) : m_name(name), m_dateFormat(dateFormat) {}

Appender::~Appender() {}

void Appender::SetName(const string& name)
{
    m_name = name;
}

const string& Appender::Name() const
{
    return m_name;
}

bool Appender::Append(const Level &level,  const std::string &content) 
{
    string line = Head() + "[" + level.Str() + "]: " + content + Tail();
    if (!DoAppend(line))
    {
        return false;
    }
    return true;
}

bool Appender::DoAppend(const std::string &content)
{
    cout << content;
    return true;
}

string Appender::Head() const
{
    return "[" + Date().Format(m_dateFormat) + "]";
}

string Appender::Tail() const
{
    return "\n";
}

