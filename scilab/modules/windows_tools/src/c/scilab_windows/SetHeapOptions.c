
/*--------------------------------------------------------------------------*/ 
#include <windows.h>
#include "SetHeapOptions.h"
/* http://msdn.microsoft.com/en-us/library/bb430720.aspx */
/*--------------------------------------------------------------------------*/ 
typedef BOOL (WINAPI *HSI) (HANDLE, HEAP_INFORMATION_CLASS ,PVOID, SIZE_T);
/*--------------------------------------------------------------------------*/ 
BOOL SetHeapOptions(void) 
{
#ifdef _DEBUG
	HMODULE hLib = LoadLibrary(TEXT("kernel32.dll"));
	if (hLib == NULL) return FALSE;
	else
	{
		BOOL fRet = FALSE;
		HSI pHsi = (HSI)GetProcAddress(hLib,"HeapSetInformation");
		if (!pHsi) 
		{
			FreeLibrary(hLib);
			return FALSE;
		}
		#ifndef HeapEnableTerminationOnCorruption
			#define HeapEnableTerminationOnCorruption (HEAP_INFORMATION_CLASS)1
		#endif

		fRet = (pHsi)(NULL,HeapEnableTerminationOnCorruption,NULL,0) ? TRUE : FALSE;

		if (hLib) FreeLibrary(hLib);

		return fRet;
   }
#else
	return FALSE;
#endif
}
/*--------------------------------------------------------------------------*/ 
