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

bool SystemInfo::GetInfo()
{
	m_name = GetName();
	m_version = GetVersion();
	m_arch = GetArch();
	return true;
}

string SystemInfo::GetName()
{
	string osName;
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
						osName = "Microsoft Windows NT 4.0";
					} else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
						osName = "Microsoft Windows 95";
					}
					break;
				case 10:
					osName = "Microsoft Windows 98";
					break;
				case 90:
					osName = "Microsoft Windows Me";
					break;
			}
			break;
		case 5:  
			switch(os.dwMinorVersion)
			{
				case 0:
					osName = "Microsoft Windows 2000";
					break;
				case 1:
					osName = "Microsoft Windows XP";
					break;
				case 2:
					if (os.wProductType == VER_NT_WORKSTATION) {
						osName = "Microsoft Windows XP";  
					} else if (GetSystemMetrics(SM_SERVERR2) == 0) {
						osName = "Microsoft Windows Server 2003";
					} else if (GetSystemMetrics(SM_SERVERR2) != 0) {
						osName = "Microsoft Windows Server 2003 R2";
					}
					break;
			}
			break;
		case 6:
			switch (os.dwMinorVersion)
			{
				case 0:
					if (os.wProductType == VER_NT_WORKSTATION) {
						osName = "Microsoft Windows Vista";
					} else {
						osName = "Microsoft Windows Server 2008";
					}
					break;
				case 1:
					if (os.wProductType == VER_NT_WORKSTATION) {
						osName = "Microsoft Windows 7";
					} else {
						osName = "Microsoft Windows Server 2008 R2";
					}
					break;
				case 2:
					if (os.wProductType == VER_NT_WORKSTATION) {
						osName = "Microsoft Windows 8";
					} else {
						osName = "Microsoft Windows Server 2012";
					}
					break;
				case 3:
					if (os.wProductType == VER_NT_WORKSTATION) {
						osName = "Microsoft Windows 8.1";
					} else {
						osName = "Microsoft Windows Server 2012 R2";
					}
					break;
			}
			break;
		case 10:
			if (os.wProductType == VER_NT_WORKSTATION) {
				osName = "Microsoft Windows 10";
			} else {
				osName = "Microsoft Windows Server 2016";
			}
			break;
		default:
			osName = "Unkown";
	}

	if (osName.empty()) {
		return "Unkown";
	}
#else
	struct utsname name;
	if (uname (&name) == -1) {
		return "Unkown";	
	}
	osName = name.sysname;
	osName += " ";
	osName += name.release;

#endif //__WINDOWS__
	return osName;
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

