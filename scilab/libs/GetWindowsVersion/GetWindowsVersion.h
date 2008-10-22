/* Allan CORNET */
/* INRIA 2006 */
#ifndef __GETWINDOWSVERSION_H__
#define __GETWINDOWSVERSION_H__
#include <windows.h>
/*-----------------------------------------------------------------------------------*/
#ifdef EXPORT_GETWINDOWSVERSION_DLL
	#define IMPORT_EXPORT_GETWINDOWSVERSION_DLL __declspec(dllexport)
#else
	#define IMPORT_EXPORT_GETWINDOWSVERSION_DLL __declspec(dllimport)
#endif

#define OS_ERROR							-1
#define OS_WIN32_WINDOWS_NT_3_51			0
#define OS_WIN32_WINDOWS_NT_4_0				1
#define OS_WIN32_WINDOWS_95					2
#define OS_WIN32_WINDOWS_98					3
#define OS_WIN32_WINDOWS_Me					4
#define OS_WIN32_WINDOWS_2000				5
#define OS_WIN32_WINDOWS_XP					6
#define OS_WIN32_WINDOWS_XP_64				7
#define OS_WIN32_WINDOWS_SERVER_2003		8
#define OS_WIN32_WINDOWS_SERVER_2003_R2		9
#define OS_WIN32_WINDOWS_SERVER_2003_64		10
#define OS_WIN32_WINDOWS_VISTA				11
#define OS_WIN32_WINDOWS_VISTA_64			12
#define OS_WIN32_WINDOWS_SERVER_2008		13
#define OS_WIN32_WINDOWS_SERVER_2008_64		14

/*-----------------------------------------------------------------------------------*/
IMPORT_EXPORT_GETWINDOWSVERSION_DLL int GetWindowsVersion(void);
IMPORT_EXPORT_GETWINDOWSVERSION_DLL BOOL IsWow64(void);
/*-----------------------------------------------------------------------------------*/
#endif /* __GETWINDOWSVERSION_H__ */