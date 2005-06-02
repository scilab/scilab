#include "GetOS.h"
/*-----------------------------------------------------------------------------------*/
static int SciPlatformId;
/*-----------------------------------------------------------------------------------*/
/*
SciPlatformId :
	VER_PLATFORM_WIN32s		Win32s on Windows 3.1.
	VER_PLATFORM_WIN32_WINDOWS	Win32 on Windows 95.
	VER_PLATFORM_WIN32_NT	Win32 on Windows NT
*/
int SciWinGetPlatformId ()
{
	OSVERSIONINFO os;
	os.dwOSVersionInfoSize = sizeof (os);
	GetVersionEx (&os);
	SciPlatformId = os.dwPlatformId;

	return SciPlatformId;
}
/*-----------------------------------------------------------------------------------*/
int GetOSVersion(void)
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(!(bOsVersionInfoEx=GetVersionEx((OSVERSIONINFO *)&osvi)))
    {
		osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
		if (!GetVersionEx((OSVERSIONINFO *)&osvi) ) 
		return OS_ERROR;
    }
	if(osvi.dwMajorVersion==3 && osvi.dwMinorVersion==51) return OS_WIN32_WINDOWS_NT_3_51;
	if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==0 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT) return OS_WIN32_WINDOWS_NT_4_0;
	if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==0 && osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS) return OS_WIN32_WINDOWS_95;
	if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==10 && osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)return OS_WIN32_WINDOWS_98;
	if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==90 && osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS) return OS_WIN32_WINDOWS_Me;
	if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==0 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)  return OS_WIN32_WINDOWS_2000;
	if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==1 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)  return OS_WIN32_WINDOWS_XP;
	if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==2 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)  return OS_WIN32_WINDOWS_SERVER_2003_FAMILY;

	return OS_ERROR;
}
/*-----------------------------------------------------------------------------------*/
