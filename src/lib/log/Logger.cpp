#include "Logger.h"

using std::map;
using std::string;

Logger::Logger(const string &name) : m_name(name) {}

Logger::~Logger() {}

void Logger::Fatal(const string &content)
{
    Log(Level(Level::FATAL), content);
}

void Logger::Error(const string &content)
{
    Log(Level(Level::ERROR), content);
}

void Logger::Warn(const string &content)
{
    Log(Level(Level::WARN), content);
}

void Logger::Notice(const string &content)
{
    Log(Level(Level::NOTICE), content);
}

void Logger::Info(const string &content)
{
    Log(Level(Level::INFO), content);
}

void Logger::Debug(const string &content)
{
    Log(Level(Level::DEBUG), content);
}

void Logger::SetLevel(const Level& level)
{
    m_level = level;
}

const Level& Logger::GetLevel() const
{
    return m_level;
}

bool Logger::AddAppender(AppenderPtr pAppender)
{
    if (pAppender)
    {
        string name = pAppender->Name();
        map<string, AppenderPtr>::iterator iter= m_appenders.find(name);
        if (m_appenders.end() == iter)
        {
            m_appenders[name] = pAppender;
            return true;
        }
    }
    return false;
}

bool Logger::RemoveAppender(const string &name)
{
    map<string, AppenderPtr>::iterator iter= m_appenders.find(name);
    if (m_appenders.end() == iter)
    {
        return false;
    }
    m_appenders.erase(iter);
    return true;
}

void Logger::Log(const Level &level, const string &content)
{
    if (m_level < level)
    {
        return ;
    }

    for (map<string, AppenderPtr>::iterator iter = m_appenders.begin(); iter != m_appenders.end(); ++iter)
    {
        iter->second->Append(level, content);
    }
}
