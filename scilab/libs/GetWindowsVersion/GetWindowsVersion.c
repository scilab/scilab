#include "GetWindowsVersion.h"
/*-----------------------------------------------------------------------------------*/
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
#define SM_SERVERR2            89
/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_GETWINDOWSVERSION_DLL int GetWindowsVersion(void)
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) return OS_ERROR;
	}

	switch (osvi.dwPlatformId)
	{
		case VER_PLATFORM_WIN32_NT:

			if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION ) 
				{
					#ifdef  _WIN64
						return OS_WIN32_WINDOWS_SEVEN_64;
					#else
						if (IsWow64()) return OS_WIN32_WINDOWS_SEVEN_64;
						else return OS_WIN32_WINDOWS_SEVEN;
					#endif
				}
				else
				{
					#ifdef  _WIN64
						return OS_WIN32_WINDOWS_SEVEN_SERVER_64;
					#else
						if (IsWow64()) return OS_WIN32_WINDOWS_SEVEN_SERVER_64;
						else return  OS_WIN32_WINDOWS_SEVEN_SERVER;
					#endif
				}
			}

			if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION ) 
				{
					#ifdef  _WIN64
						return OS_WIN32_WINDOWS_VISTA_64;
					#else
						if (IsWow64()) return OS_WIN32_WINDOWS_VISTA_64;
						else return OS_WIN32_WINDOWS_VISTA;
					#endif
				}
				else
				{
					#ifdef  _WIN64
						return OS_WIN32_WINDOWS_SERVER_2008_64;
					#else
						if (IsWow64()) return OS_WIN32_WINDOWS_SERVER_2008_64;
						else return  OS_WIN32_WINDOWS_SERVER_2008;
					#endif
				}
			}

			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
			{
				SYSTEM_INFO si;
				PGNSI pGNSI;
				pGNSI = (PGNSI) GetProcAddress(GetModuleHandle("kernel32.dll"),"GetNativeSystemInfo");
				if(NULL != pGNSI) pGNSI(&si);

				if( GetSystemMetrics(SM_SERVERR2) ) return OS_WIN32_WINDOWS_SERVER_2003_R2;
				else
					if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64) return OS_WIN32_WINDOWS_XP_64;
					else return OS_WIN32_WINDOWS_SERVER_2003;
			}

			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 ) return OS_WIN32_WINDOWS_XP;
			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 ) return OS_WIN32_WINDOWS_2000;
			if ( osvi.dwMajorVersion == 4 ) return OS_WIN32_WINDOWS_NT_4_0;
			if ( osvi.dwMajorVersion == 3 && osvi.dwMinorVersion == 51 ) return OS_WIN32_WINDOWS_NT_3_51;
		break;

		case VER_PLATFORM_WIN32_WINDOWS:
			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) return OS_WIN32_WINDOWS_95;
			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) return OS_WIN32_WINDOWS_98;
			if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) return OS_WIN32_WINDOWS_Me;
		break;
	}
	return OS_ERROR;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsWow64(void)
{
	BOOL bIsWow64 = FALSE;
	LPFN_ISWOW64PROCESS fnIsWow64Process;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle("kernel32"),"IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
			
		}
	}
	return bIsWow64;
}
/*-----------------------------------------------------------------------------------*/

