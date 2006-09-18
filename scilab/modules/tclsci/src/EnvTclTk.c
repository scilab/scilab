/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#include <windows.h>
#else
#define MAX_PATH 512
#include "machine.h"
#endif

#include <stdio.h>
#include "Errors.h"
#ifdef WITH_TK
	#include "tcl.h"
	#include "tk.h"
#endif
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
extern BOOL ConvertPathUnixToWindowsFormat(char *pathunix,char *pathwindows);
/*-----------------------------------------------------------------------------------*/
BOOL Set_TCL_LIBRARY_PATH(char *DefaultPath)
{
	BOOL bOK=FALSE;
	
	int major=8;
	int minor=4; // Par defaut
	int patchLevel=0;
	int type=0;
	
	char env[MAX_PATH + 1 + 10];
	
	char ShortPath[MAX_PATH+1];
	char *CopyOfDefaultPath=NULL;

	CopyOfDefaultPath=MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));

	
	#ifdef WITH_TK
	Tcl_GetVersion(&major, &minor, &patchLevel, &type);
	#endif

	/* to be sure that it's windows format */
	/* c:\progra~1\scilab-5.0\modules\tclsci\tcl\tcl8.4 */
	if (GetShortPathName(DefaultPath,ShortPath,MAX_PATH)==0)
	{
		fprintf(stderr,"\n%s%s%s.\n",TCL_ERROR35,TCL_ERROR37,DefaultPath);
		if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}

		ConvertPathUnixToWindowsFormat(ShortPath,CopyOfDefaultPath);
		wsprintf (env, "TCL_LIBRARY=%s\\modules\\tclsci\\tcl\\tcl%d.%d",CopyOfDefaultPath,major,minor);

		if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
	}
	else
	{
		ConvertPathUnixToWindowsFormat(ShortPath,CopyOfDefaultPath);
		wsprintf (env, "TCL_LIBRARY=%s\\modules\\tclsci\\tcl\\tcl%d.%d",CopyOfDefaultPath,major,minor);
		
		if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
	}

	if (_putenv (env))
	{
		bOK=FALSE;
	}
	else
	{
		bOK=TRUE;
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL Set_TK_LIBRARY_PATH(char *DefaultPath)
{
	BOOL bOK=FALSE;
	char env[MAX_PATH + 1 + 10];
	char ShortPath[MAX_PATH+1];
	char *CopyOfDefaultPath=NULL;

	int major=8;
	int minor=4; // Par defaut
	int patchLevel=0;
	int type=0;

	CopyOfDefaultPath=MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));

	#ifdef WITH_TK
	Tcl_GetVersion(&major, &minor, &patchLevel, &type);
	#endif

	/* to be sure that it's windows format */
	/* c:\progra~1\scilab-3.1\modules\tclsci\tcl\tk8.4 */
	if (GetShortPathName(DefaultPath,ShortPath,MAX_PATH)==0)
	{
		fprintf(stderr,"\n%s%s%s.\n",TCL_ERROR35,TCL_ERROR38,DefaultPath);
		if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}

		ConvertPathUnixToWindowsFormat(ShortPath,CopyOfDefaultPath);
		wsprintf (env, "TK_LIBRARY=%s\\modules\\tclsci\\tcl\\tk%d.%d",CopyOfDefaultPath,major,minor);

		if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
		
	}
	else
	{
		ConvertPathUnixToWindowsFormat(ShortPath,CopyOfDefaultPath);
		wsprintf (env, "TK_LIBRARY=%s\\modules\\tclsci\\tcl\\tk%d.%d",CopyOfDefaultPath,major,minor);

		if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
	}

	if (_putenv (env))
	{
		bOK=FALSE;
	}
	else
	{
		bOK=TRUE;
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
