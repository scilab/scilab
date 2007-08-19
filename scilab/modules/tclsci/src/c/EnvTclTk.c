/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "machine.h"

#include <stdio.h>
#include "tcl.h"
#include "tk.h"
#include "MALLOC.h"
#include "../../localization/includes/QueryStringError.h"
#include "../../fileio/includes/ConvertPathUnixToWindows.h"
/*-----------------------------------------------------------------------------------*/
BOOL Set_TCL_LIBRARY_PATH(char *DefaultPath)
{
	BOOL bOK=FALSE;
	
	int major=8;
	int minor=4; /* Par defaut */
	int patchLevel=0;
	int type=0;
	/* @TODO : WTF is 10 ? */
	char env[PATH_MAX + 1 + 10];
	
	char ShortPath[PATH_MAX+1];
	char *CopyOfDefaultPath=NULL;

	CopyOfDefaultPath=MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));

	
	Tcl_GetVersion(&major, &minor, &patchLevel, &type);

	/* to be sure that it's windows format */
	/* c:\progra~1\scilab-5.0\modules\tclsci\tcl\tcl8.4 */
	if (GetShortPathName(DefaultPath,ShortPath,PATH_MAX)==0)
	{
		char *msg1=QueryStringError("tclsci_error_40");
		char *msg2=QueryStringError("tclsci_error_43");
		fprintf(stderr,"\n%s\n%s%s.\n",msg1,msg2,DefaultPath);
		if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
		if (msg1) {FREE(msg1);msg1=NULL;}
		if (msg2) {FREE(msg2);msg2=NULL;}

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
	char env[PATH_MAX + 1 + 10];
	char ShortPath[PATH_MAX+1];
	char *CopyOfDefaultPath=NULL;

	int major=8;
	int minor=4; /* Par defaut */
	int patchLevel=0;
	int type=0;

	CopyOfDefaultPath=MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));

	Tcl_GetVersion(&major, &minor, &patchLevel, &type);

	/* to be sure that it's windows format */
	/* c:\progra~1\scilab-3.1\modules\tclsci\tcl\tk8.4 */
	if (GetShortPathName(DefaultPath,ShortPath,PATH_MAX)==0)
	{
		char *msg1=QueryStringError("tclsci_error_41");
		char *msg2=QueryStringError("tclsci_error_42");
		fprintf(stderr,"\n%s\n%s%s.\n",msg1,msg2,DefaultPath);
		if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}
		if (msg1){FREE(msg1);msg1=NULL;}
		if (msg2){FREE(msg2);msg2=NULL;}

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
