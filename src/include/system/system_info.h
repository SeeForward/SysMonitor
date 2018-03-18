//Date:2016-3-25
//Author:lidengke
//Desc: Operating System Info

#ifndef __SYSTEM_INFO_H__
#define __SYSTEM_INFO_H__

#include "type_def.h"
#include <string>

using std::string;

class SystemInfo
{
public:
	string Name();
	string Desc();
	string Version();
	string Arch();

private:
	static string GetName();
	static string GetDesc();
	static string GetVersion();
	static string GetArch();

private:
	string m_name;
	string m_desc;
	string m_version;
	string m_arch;
};

#endif //__SYSTEM_INFO_H__
