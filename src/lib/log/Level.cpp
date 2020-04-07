#include "Level.h"

using std::string;

const char* Level::s_LevelStr[Level::MAX] = {"FATAL", "ERROR", "WARN", "NOTICE", "INFO", "DEBUG"};


Level::Level(Level::Type level) : m_level(level) {}

string Level::Str() const
{
    return s_LevelStr[m_level];
}

const Level& Level::operator = (Level::Type level)
{
    m_level = level;
}

const Level& Level::operator = (const Level &other)
{
    if (this != &other)
    {
        m_level = other.m_level;
    }
    return *this;
}

bool Level::operator == (const Level &other) const
{
    return m_level == other.m_level;
}

bool Level::operator != (const Level &other) const
{
    return m_level != other.m_level;
}

bool Level::operator <= (const Level &other) const
{
    return m_level <= other.m_level;
}

bool Level::operator >= (const Level &other) const
{
    return m_level >= other.m_level;
}

bool Level::operator < (const Level &other) const
{
    return m_level < other.m_level;
}

bool Level::operator > (const Level &other) const
{
    return m_level > other.m_level;
}
