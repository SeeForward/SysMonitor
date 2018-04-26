#ifndef __PATH_H__
#define __PATH_H__

#include <string>

class Path
{
public:
	Path();
	Path(const std::string &path);

	std::string Str() const;

	void SetPath(const std::string &path);

    bool IsExist() const;

    bool IsAbsolute() const;

    bool IsDirectory() const;

	std::string ExtractFileName() const;

	std::string ExtractPath() const;

    bool operator == (const Path &other) const;

    bool operator != (const Path &other) const;

    static std::string GetExecuteFilePath();

private:
	std::string m_path;
};

#endif //__PATH_H__
