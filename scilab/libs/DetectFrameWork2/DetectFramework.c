/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET*/
/*-----------------------------------------------------------------------------------*/ 
#include "DetectFramework.h"
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_DETECTFRAMEWORK_DLL int DetectFrameWorkNET2(void)
{
	int bOK=FALSE;
	HKEY key;
	DWORD result=0;

	result=RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\.NETFramework\\policy\\v2.0", 0, KEY_QUERY_VALUE , &key);
	
	if ( result == ERROR_SUCCESS )
	{
		 RegCloseKey(key);
		 bOK=TRUE;
	}
	
	return bOK;
	
}
/*-----------------------------------------------------------------------------------*/ 
