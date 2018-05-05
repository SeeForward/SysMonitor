#ifndef __APPEDNER_H__
#define __APPEDNER_H__

#include <string>
#include "Level.h"
#include "SmartPtr.h"
#include "DateFormat.h"

class Appender
{
public:
    Appender(const std::string &name, const DateFormat &dateFormat);

    virtual ~Appender();

    void SetName(const std::string &name);

    const std::string& Name() const;

    bool Append(const Level &level, const std::string &content);

protected:
    virtual bool DoAppend(const std::string &content);

    virtual std::string Head() const;
    virtual std::string Tail() const;

protected:
    std::string m_name;
    DateFormat  m_dateFormat;
};

typedef SmartPtr<Appender> AppenderPtr;

#endif //__APPEDNER_H__
