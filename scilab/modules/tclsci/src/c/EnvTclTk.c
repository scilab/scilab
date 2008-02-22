/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "machine.h"

#include <stdio.h>
#include "tcl.h"
#include "tk.h"
#include "MALLOC.h"
#include "../../localization/includes/localization.h"
#include "../../fileio/includes/ConvertPathUnixToWindows.h"
/*--------------------------------------------------------------------------*/
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
		fprintf(stderr,"\n%s\n%s%s.\n",_("Incorrect TCL_LIBRARY environment variable"),_("TCL_LIBRARY has been redefined to "),DefaultPath);
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
/*--------------------------------------------------------------------------*/
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
		fprintf(stderr,"\n%s\n%s%s.\n",_("Incorrect TK_LIBRARY environment variable"),_("TK_LIBRARY has been redefined to "),DefaultPath);
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
/*--------------------------------------------------------------------------*/
