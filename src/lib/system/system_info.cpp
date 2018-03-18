//Date:2016-3-25
//Author:lidengke

//2016-9-30		GetName support Windows 8, Windows 8.1 and Windows 10.

#include "type_def.h"
#include "system_info.h"

#ifdef __WINDOWS__
#	include <Windows.h>
#else
#	include <unistd.h>
#	include <sys/utsname.h>
#endif //__WINDOWS__

string SystemInfo::Name()
{
	if (m_name.empty())
	{
		m_name = GetName();
	}

	return m_name;
}

string SystemInfo::Desc()
{
	if (m_desc.empty())
	{
		m_desc = GetDesc();
	}

	return m_desc;
}

string SystemInfo::Version()
{
	if (m_version.empty())
	{
		m_version = GetVersion();
	}

	return m_version;
}

string SystemInfo::Arch()
{
	if (m_arch.empty())
	{
		m_arch = GetArch();
	}

	return m_arch;
}

string SystemInfo::GetName()
{
#ifdef __WINDOWS__
	return "Windows";
#else
	return "Linux";
#endif
}

string SystemInfo::GetDesc()
{
	string osDesc;
#ifdef __WINDOWS__
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize=sizeof(os);
	if (!::GetVersionEx((LPOSVERSIONINFO)&os)) {
		return "Unkown";
	}

	switch (os.dwMajorVersion)
	{
		case 4:
			switch (os.dwMinorVersion)
			{
				case 0:
					if (os.dwPlatformId == VER_PLATFORM_WIN32_NT) {
						osDesc = "Microsoft Windows NT 4.0";
					} else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
						osDesc = "Microsoft Windows 95";
					}
					break;
				case 10:
					osDesc = "Microsoft Windows 98";
					break;
				case 90:
					osDesc = "Microsoft Windows Me";
					break;
			}
			break;
		case 5:  
			switch(os.dwMinorVersion)
			{
				case 0:
					osDesc = "Microsoft Windows 2000";
					break;
				case 1:
					osDesc = "Microsoft Windows XP";
					break;
				case 2:
					if (os.wProductType == VER_NT_WORKSTATION) {
						osDesc = "Microsoft Windows XP";  
					} else if (GetSystemMetrics(SM_SERVERR2) == 0) {
						osDesc = "Microsoft Windows Server 2003";
					} else if (GetSystemMetrics(SM_SERVERR2) != 0) {
						osDesc = "Microsoft Windows Server 2003 R2";
					}
					break;
			}
			break;
		case 6:
			switch (os.dwMinorVersion)
			{
				case 0:
					if (os.wProductType == VER_NT_WORKSTATION) {
						osDesc = "Microsoft Windows Vista";
					} else {
						osDesc = "Microsoft Windows Server 2008";
					}
					break;
				case 1:
					if (os.wProductType == VER_NT_WORKSTATION) {
						osDesc = "Microsoft Windows 7";
					} else {
						osDesc = "Microsoft Windows Server 2008 R2";
					}
					break;
				case 2:
					if (os.wProductType == VER_NT_WORKSTATION) {
						osDesc = "Microsoft Windows 8";
					} else {
						osDesc = "Microsoft Windows Server 2012";
					}
					break;
				case 3:
					if (os.wProductType == VER_NT_WORKSTATION) {
						osDesc = "Microsoft Windows 8.1";
					} else {
						osDesc = "Microsoft Windows Server 2012 R2";
					}
					break;
			}
			break;
		case 10:
			if (os.wProductType == VER_NT_WORKSTATION) {
				osDesc = "Microsoft Windows 10";
			} else {
				osDesc = "Microsoft Windows Server 2016";
			}
			break;
		default:
			osDesc = "Unkown";
	}

	if (osDesc.empty()) {
		return "Unkown";
	}
#else
	struct utsname name;
	if (uname (&name) == -1) {
		return "Unkown";	
	}
	osDesc = name.sysname;
	osDesc += " ";
	osDesc += name.release;

#endif //__WINDOWS__
	return osDesc;
}

string SystemInfo::GetVersion() 
{
#ifdef __WINDOWS__
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize = sizeof(os);
	if (!::GetVersionEx((LPOSVERSIONINFO)&os)) {
		return "";
	}

	char buf[64];
	sprintf(buf, "%d.%d.%d", os.dwMajorVersion, os.dwMinorVersion, os.dwBuildNumber);

	return buf;
#else
	struct utsname name;
	if (uname (&name) == -1) {
		return "";	
	}

	return name.version;

#endif //__WINDOWS__
}

string SystemInfo::GetArch()
{
#ifdef __WINDOWS__
	string arch;
	SYSTEM_INFO si;
	::GetNativeSystemInfo(&si);

	switch (si.wProcessorArchitecture) 
	{
		case PROCESSOR_ARCHITECTURE_AMD64:
			arch = "x86_64";
			break;
		case PROCESSOR_ARCHITECTURE_ARM:
			arch = "ARM";
			break;
		case PROCESSOR_ARCHITECTURE_IA64:
			arch = "IA64";
			break;
		case PROCESSOR_ARCHITECTURE_INTEL:
			arch = "x86";
			break;
		default:
			arch = "";
			break;
	}

	return arch;
#else
	struct utsname name;
	if (uname (&name) == -1) {
		return "";	
	}

	return name.machine;
#endif //__WINDOWS__
}

