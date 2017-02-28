#include "memory_info.h"
#include "memory_state.h"

#ifdef __WINDOWS__
#	include <Windows.h>
#	include <TlHelp32.h>
#	include <Psapi.h>
#	include <comdef.h>
#	include <WbemIdl.h>
#	include <sstream>

	using std::stringstream;
#else
#	include <unistd.h>
#	include <stdio.h>
#	include <string.h>
#	include <dirent.h>
#	include <stdlib.h>
#endif //__WINDOWS__

bool MemoryInfo::GetInfo() 
{
#ifdef __WINDOWS__
	vector<string> vecQuery;
	vecQuery.push_back("Capacity");
	vecQuery.push_back("Manufacturer");
	vecQuery.push_back("MemoryType");
	vecQuery.push_back("PartNumber");
	vecQuery.push_back("Speed");

	vector<string> vecRet;
	if (!GetMemoryItemsInfo(vecQuery, vecRet)) {
		return false;
	}

	if (vecRet.size() < vecQuery.size()) {
		return false;
	}

	if (!sscanf(vecRet[0].c_str(), "%llu", &m_singleBytes)) {
		m_singleBytes = 0;
	}

	m_manufacturer = vecRet[1].empty() ? "Unknow" : vecRet[1];
	
	unsigned int u = 0;
	sscanf(vecRet[2].c_str(), "%u", &u);
	switch (u)
	{
		case 0:
			m_type = "Unknow";
			break;
		case 1:
			m_type = "Other";
			break;
		case 2:
			m_type = "DRAM";
			break;
		case 3:
			m_type = "Synchronous DRAM";
			break;
		case 4:
			m_type = "Cache DRAM";
			break;
		case 5:
			m_type = "EDO";
			break;
		case 6:
			m_type = "EDRAM";
			break;
		case 7:
			m_type = "VRAM";
			break;
		case 8:
			m_type = "SRAM";
			break;
		case 9:
			m_type = "RAM";
			break;
		case 10:
			m_type = "ROM";
			break;
		case 11:
			m_type = "Flash";
			break;
		case 12:
			m_type = "EEPROM";
			break;
		case 13:
			m_type = "FEPROM";
			break;
		case 14:
			m_type = "EPROM";
			break;
		case 15:
			m_type = "CDRAM";
			break;
		case 16:
			m_type = "3DRAM";
			break;
		case 17:
			m_type = "SDRAM";
			break;
		case 18:
			m_type = "SGRAM";
			break;
		case 19:
			m_type = "RDRAM";
			break;
		case 20:
			m_type = "DDR";
			break;
		case 21:
			m_type = "DDR2";
			break;
		case 22:
			m_type = "DDR2";
			break;
		case 23:
			m_type = "DDR2 FB-DIMM";
			break;
		case 24:
			m_type = "DDR3";
			break;
		case 25:
			m_type = "FBD2";
			break;
	}

	m_model = vecRet[3].empty() ? "Unknow" : vecRet[3];

	
	if (!sscanf(vecRet[4].c_str(), "%d", &m_speed)) {
		m_speed = 0;
	} else if (0 == u) {
		if(m_speed == 533 || m_speed == 667 || m_speed == 800 || m_speed == 1066 ) {
			m_type = "DDR2";
		} else if (m_speed == 1333 || m_speed == 1600 || m_speed == 2133){
			m_type = "DDR3";
		}
	}
#else
	FILE *fp = popen("decode -t memory 2> /dev/null | grep -m1 -A 15 'Memory Device'", "r");
	char buf[256];
	if (!fgets(buf, sizeof(buf), fp)) 
	{
		pclose(fp);
		fp = popen("/usr/sbin/decode -t memory 2> /dev/null | grep -m1 -A 15 'Memory Device'", "r");
		if (!fgets(buf, sizeof(buf), fp)) {
			pclose(fp);
			return false;
		}
	}

	int getItemNum = 0;
	while (fgets(buf, sizeof(buf), fp)) 
	{
		if(strstr(buf, "Size:")) {
			sscanf(buf, "%*s %"PRIu64, &m_singleBytes);
			m_singleBytes *= 1024 * 1024;
			++getItemNum;
		} else if(strstr(buf, "Type:")) {
			char type[32];
			sscanf(buf, "%*s %s", type);
			m_type = type;
			++getItemNum;
		} else if(strstr(buf, "Speed:")) {
			sscanf(buf, "%*s %d", &m_speed);
			++getItemNum;
		} else if(strstr(buf, "Manufacturer:")) {
			string strTmp = buf;
			int pos = strTmp.find_first_of(":");
			m_manufacturer = strTmp.substr(pos + 2, strTmp.size() - pos - 3);
			++getItemNum;
		} else if(strstr(buf, "Serial Number:")) {
			string strTmp = buf;
			int pos = strTmp.find_first_of(":");
			m_model = strTmp.substr(pos + 2, strTmp.size() - pos - 3);
			++getItemNum;
		}
	}
	pclose(fp);

	if (getItemNum < 5) {
		return false;
	}
#endif //__WINDOWS__
	MemoryState ms;
	if (!ms.GetMemoryState()) {
		return false;
	}
	
	m_totalBytes = ms.m_totalPhys;
	if (m_totalBytes && m_singleBytes) 
	{
		m_number = (int32_t)(m_totalBytes / m_singleBytes);
		if (m_totalBytes % m_singleBytes) {
			++m_number;
		}
		m_totalBytes = m_singleBytes * m_number;
	}
	return true;
}

#ifdef __WINDOWS__
bool MemoryInfo::GetMemoryItemsInfo(const vector<string>& vecClsMember, vector<string>& vecRet)
{
	HRESULT hr = ::CoInitializeEx(0,COINIT_MULTITHREADED);  
	if (!SUCCEEDED(hr) && RPC_E_CHANGED_MODE != hr) {
		return false;
	}
	hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);  
	if (!SUCCEEDED(hr)) {
		return false;
	}
	IWbemLocator* pWbemLoc = NULL;
	hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pWbemLoc);  
	if (!SUCCEEDED(hr)) {
		return false;
	}
	IWbemServices* pWbemSvc = NULL; 
	hr = pWbemLoc->ConnectServer(bstr_t("ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pWbemSvc);  
	if (!SUCCEEDED(hr)) {
		return false;
	}

	hr = CoSetProxyBlanket(pWbemSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);  
	if (!SUCCEEDED(hr)) {
		return false;
	} 

	IEnumWbemClassObject* pEnumClsObj = NULL;
	string query = string("SELECT * FROM Win32_PhysicalMemory");  
	hr = pWbemSvc->ExecQuery(bstr_t("WQL"), bstr_t(query.c_str()),WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 0,&pEnumClsObj);  
	if (!SUCCEEDED(hr)) { 
		return false;
	}

	
	ULONG uReturn = 0;
	IWbemClassObject* pWbemClsObj = NULL;
	hr = pEnumClsObj->Next(WBEM_INFINITE, 1, &pWbemClsObj, &uReturn);  
	if (!SUCCEEDED(hr) || uReturn <= 0) {
		return false;
	}
	
	VARIANT var; 
	VariantInit(&var);
	char buf[512] = {0};
	for(size_t i = 0; i < vecClsMember.size(); ++i)  
	{  
		hr = pWbemClsObj->Get(bstr_t(vecClsMember[i].c_str()), 0, &var,0,0);  
		if (!SUCCEEDED(hr)) {
			vecRet.push_back("");
			continue;
		}
		
		switch(var.vt)
		{
			case VT_BSTR:  
				::WideCharToMultiByte(CP_ACP, NULL, var.bstrVal, (int)wcslen(var.bstrVal) + 1, buf, 512, 0, 0);
				break;
			case VT_I4:
				sprintf(buf, "%d", var.lVal);
				break;
			case VT_UI1:
				sprintf(buf, "%d", var.bVal);
				break;
			case VT_UI4:
				sprintf(buf, "%lu", var.ulVal);
				break;
			default:
				buf[0] = '\0';
				break;
		}

		vecRet.push_back(buf);
		VariantClear(&var);
	}

	pWbemClsObj->Release();  
	pEnumClsObj->Release();
	pWbemSvc->Release();
	pWbemLoc->Release();

	::CoUninitialize();
	return true;
}
#endif //__WINDOWS__

