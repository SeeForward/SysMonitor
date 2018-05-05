
#include "FileAppender.h"
#include <iostream>

using std::string;
using std::ios;

FileAppender::FileAppender(const string &name, const DateFormat &dateFormat, const string &filePath) : Appender(name, dateFormat)
{
    m_ostream.open(filePath, ios::out|ios::app);
}

FileAppender::~FileAppender()
{
    m_ostream.close();
}

bool FileAppender::DoAppend(const string &content)
{
    m_ostream << content;
    return true;
}
