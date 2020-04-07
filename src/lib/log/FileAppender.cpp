
#include "FileAppender.h"
#include <iostream>

using std::string;
using std::ios;
using std::fstream;

FileAppender::FileAppender(const string &name, const DateFormat &dateFormat, const string &filePath) : Appender(name, dateFormat)
{
    m_ostream.open(filePath.c_str(), fstream::out|fstream::app);
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
