#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <map>
#include "Appender.h"
#include "SmartPtr.h"

class Logger
{
public:
    Logger(const std::string &name = std::string());
    ~Logger();

    void Fatal(const std::string &content);
    void Error(const std::string &content);
    void Warn(const std::string &content);
    void Notice(const std::string &content);
    void Info(const std::string &content);
    void Debug(const std::string &content);

    void SetLevel(const Level& level);
    const Level& GetLevel() const;

    bool AddAppender(AppenderPtr pAppender);
    bool RemoveAppender(const std::string &name);

private:
    void Log(const Level &level, const std::string &content);

private:
    std::string                        m_name;
    Level                              m_level;
    std::map<std::string, AppenderPtr> m_appenders;
};

typedef SmartPtr<Logger> LoggerPtr;

#endif //__LOGGER_H__
