/* Allan CORNET */
/* INRIA 2005 */
#ifndef __GETOSWIN_H__
#define __GETOSWIN_H__
#include <windows.h>
/*-----------------------------------------------------------------------------------*/
#define OS_ERROR                       -1
#define OS_WIN32_WINDOWS_NT_3_51        0
#define OS_WIN32_WINDOWS_NT_4_0         1
#define OS_WIN32_WINDOWS_95			    2
#define OS_WIN32_WINDOWS_98				3
#define OS_WIN32_WINDOWS_Me				4
#define OS_WIN32_WINDOWS_2000			5
#define OS_WIN32_WINDOWS_XP				6
#define OS_WIN32_WINDOWS_SERVER_2003_FAMILY 7
/*-----------------------------------------------------------------------------------*/
int GetOSVersion(void);
int SciWinGetPlatformId ();
/*-----------------------------------------------------------------------------------*/
#endif /* __GETOSWIN_H__ */