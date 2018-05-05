#ifndef __FILE_APPENDER_H__
#define __FILE_APPENDER_H__

#include <string>
#include <fstream>
#include "Appender.h"


class FileAppender : public Appender
{
public:
    FileAppender(const std::string &name, const DateFormat &dateformat, const std::string &FilePath);

    virtual ~FileAppender();

protected:
    virtual bool DoAppend(const std::string &content);

private:
    std::ofstream m_ostream;
};


#endif //__FILE_APPENDER_H__
