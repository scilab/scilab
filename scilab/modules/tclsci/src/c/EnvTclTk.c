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

#include <windows.h>
#include <stdio.h>
#include <tcl.h>
#include <tk.h>

#include "machine.h"
#include "MALLOC.h"
#include "GlobalTclInterp.h"
#include "../../localization/includes/localization.h"
#include "../../fileio/includes/ConvertPathUnixToWindows.h"
/*--------------------------------------------------------------------------*/
BOOL Set_TCL_LIBRARY_PATH(char *DefaultPath)
{
#define TCL_LIBRARY "TCL_LIBRARY"
#define TCL_ENV "env"
#define TCL_LIBRARY_FORMAT "%s\\modules\\tclsci\\tcl\\tcl%d.%d"

	BOOL bOK=FALSE;
	int major = 8;
	int minor = 4; /* default */
	int patchLevel = 0;
	int type = 0;

	char TCL_LIBRARY_PATH[PATH_MAX];
	char ShortPath[PATH_MAX];
	char *CopyOfDefaultPath=NULL;

	CopyOfDefaultPath = MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));
	if (CopyOfDefaultPath == NULL)
	{
		return FALSE;
	}

	Tcl_GetVersion(&major, &minor, &patchLevel, &type);

	/* to be sure that it's windows format */
	/* c:\progra~1\scilab-5.0\modules\tclsci\tcl\tcl8.4 */

	if (GetShortPathName(DefaultPath,ShortPath,PATH_MAX)==0)
	{
		fprintf(stderr,"\n%s\n%s%s.\n",_("Incorrect TCL_LIBRARY environment variable"),_("TCL_LIBRARY has been redefined to "),DefaultPath);
		ConvertPathUnixToWindowsFormat(ShortPath,CopyOfDefaultPath);
		wsprintf (TCL_LIBRARY_PATH, TCL_LIBRARY_FORMAT,CopyOfDefaultPath,major,minor);
	}
	else
	{
		ConvertPathUnixToWindowsFormat(ShortPath,CopyOfDefaultPath);
		wsprintf (TCL_LIBRARY_PATH, TCL_LIBRARY_FORMAT,CopyOfDefaultPath,major,minor);
	}

	if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}

	if ( SetEnvironmentVariable(TCL_LIBRARY,TCL_LIBRARY_PATH) )
	{
		if (getTclInterp() != NULL)
		{
			releaseTclInterp();
			Tcl_SetVar2(getTclInterp(), TCL_ENV, TCL_LIBRARY, TCL_LIBRARY_PATH, TCL_GLOBAL_ONLY);
			bOK = TRUE;
		}
		else
		{
			bOK = FALSE;
		}
		releaseTclInterp();
	}
	else
	{
		bOK = FALSE;
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL Set_TK_LIBRARY_PATH(char *DefaultPath)
{
#define TK_LIBRARY "TK_LIBRARY"
#define TCL_ENV "env"
#define TK_LIBRARY_FORMAT "%s\\modules\\tclsci\\tcl\\tk%d.%d"

	BOOL bOK = FALSE;
	char TK_LIBRARY_PATH[PATH_MAX];
	char ShortPath[PATH_MAX];
	char *CopyOfDefaultPath=NULL;

	int major = 8;
	int minor = 4; /* default */
	int patchLevel = 0;
	int type = 0;

	CopyOfDefaultPath = MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));

	if (CopyOfDefaultPath == NULL)
	{
		return FALSE;
	}

	Tcl_GetVersion(&major, &minor, &patchLevel, &type);

	/* to be sure that it's windows format */
	/* c:\progra~1\scilab-5.0\modules\tclsci\tcl\tk8.4 */

	if (GetShortPathName(DefaultPath,ShortPath,PATH_MAX)==0)
	{
		fprintf(stderr,"\n%s\n%s%s.\n",_("Incorrect TK_LIBRARY environment variable"),_("TK_LIBRARY has been redefined to "),DefaultPath);
		ConvertPathUnixToWindowsFormat(ShortPath,CopyOfDefaultPath);
		wsprintf (TK_LIBRARY_PATH, TK_LIBRARY_FORMAT,CopyOfDefaultPath,major,minor);
	}
	else
	{
		ConvertPathUnixToWindowsFormat(ShortPath,CopyOfDefaultPath);
		wsprintf (TK_LIBRARY_PATH, TK_LIBRARY_FORMAT,CopyOfDefaultPath,major,minor);
	}

	if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}

	if ( SetEnvironmentVariable(TK_LIBRARY,TK_LIBRARY_PATH) )
	{
		if (getTclInterp() != NULL)
		{
			releaseTclInterp();
			Tcl_SetVar2(getTclInterp(), TCL_ENV, TK_LIBRARY, TK_LIBRARY_PATH, TCL_GLOBAL_ONLY);
			bOK = TRUE;
		}
		else
		{
			bOK = FALSE;
		}
		releaseTclInterp();
	}
	else
	{
		bOK = FALSE;
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/
