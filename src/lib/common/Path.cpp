#include "Path.h"
#include "Const.h"
#include "StringUtil.h"

#ifdef __WINDOWS__
#	include <windows.h>			//for GetModuleFileName
#else //__LINUX__
#	include <stdio.h>			//for sprintf
#	include <unistd.h>			//for readlink
#   include <dirent.h>
#endif

using std::string;

Path::Path() {}

Path::Path(const string &path)
{
	SetPath(path);
}

string Path::Str() const
{
	return m_path;
}

void Path::SetPath(const string &path)
{
	m_path = StringUtil::Trim(path);
}

bool Path::IsExist() const
{
    if (m_path.empty())
    {
        return false;
    }
#ifdef __WINDOWS__
#else //__LINUX__
    if (0 != access(m_path.c_str(), F_OK))
    {
        return false;
    }
#endif
    return true;
}

bool Path::IsAbsolute() const
{
#ifdef __WINDOWS__
    if (m_path.size() < 3)
    {
        return false;
    }
    if (!('a' <= m_path[0] && m_path[0] <= 'Z'))
    {
        return false;
    }
    if (':' != m_path[1])
    {
        return false;
    }
    if (PATH_SEPERATOR != m_path[2])
    {
        return false;
    }
#else //__LINUX__
    if (m_path.empty())
    {
        return false;
    }
    if (PATH_SEPERATOR != m_path[0])
    {
        return false;
    }
#endif
    return true;
}

bool Path::IsDirectory() const
{
    if (m_path.empty())
    {
        return false;
    }
#ifdef __WINDOWS__
#else //__LINUX__
    DIR *dir = opendir(m_path.c_str());
    if (NULL == dir)
    {
        return false;
    }
    closedir(dir);
#endif
    return true;
}

string Path::ExtractFileName() const
{
	size_t pos = m_path.find_last_of(PATH_SEPERATOR);
	if (string::npos != pos && m_path.size() != pos)
    {
		return m_path.substr(pos + 1);
    }

	return m_path;
}

string Path::ExtractPath() const
{
	size_t pos = m_path.find_last_of(PATH_SEPERATOR);
	if (string::npos != pos && m_path.size() != pos)
    {
		return m_path.substr(0, pos + 1);
    }

	return m_path;
}

bool Path::operator == (const Path &other) const
{
    return (m_path == other.m_path);
}

bool Path::operator != (const Path &other) const
{
    return (m_path != other.m_path);
}

string Path::GetExecuteFilePath()
{
	char path[1024] = {0};

#ifdef __WINDOWS__
	GetModuleFileName(NULL, path, sizeof(path)/sizeof(path[0]) - 1);

#else //__LINUX__
	char link[1024] = {0};
	sprintf(link, "/proc/%d/exe", getpid());
	int ret = readlink(link, path, sizeof(path)/sizeof(path[0]) - 1);
	if (ret < 0)
    {
		return "";
	}
#endif

	return string(path);
}