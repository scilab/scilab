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
#include "EnvTclTk.h"
#include "GlobalTclInterp.h"
#include "localization.h"
#include "ConvertSlash.h"
/*--------------------------------------------------------------------------*/
extern void	TclSetLibraryPath(Tcl_Obj * pathPtr);
/*--------------------------------------------------------------------------*/
BOOL SetTclTkEnvironment(char *DefaultPath)
{
	#define TCL_LIBRARY "TCL_LIBRARY"
	#define TCL_LIBRARY_FORMAT "%s/modules/tclsci/tcl/tcl%d.%d"

	#define TK_LIBRARY "TK_LIBRARY"
	#define TK_LIBRARY_FORMAT "%s/modules/tclsci/tcl/tk%d.%d"

	int tcl_major = 8;
	int tcl_minor = 4; /* default */
	int tcl_patchLevel = 0;
	int tcl_type = 0;

	BOOL bOK = TRUE;

	char TCL_LIBRARY_PATH[PATH_MAX];
	char TK_LIBRARY_PATH[PATH_MAX];
	char tcl_encoding[PATH_MAX];
	
	char ShortPath[PATH_MAX];
	char *CopyOfDefaultPath = NULL;

	Tcl_Obj *pathPtr = NULL;
	Tcl_Obj *objPtr = NULL;

	CopyOfDefaultPath = MALLOC(((int)strlen(DefaultPath)+1)*sizeof(char));
	if (CopyOfDefaultPath == NULL) return FALSE;

	Tcl_GetVersion(&tcl_major, &tcl_minor, &tcl_patchLevel, &tcl_type);

	GetShortPathName(DefaultPath,ShortPath,PATH_MAX);
	AntislashToSlash(ShortPath,CopyOfDefaultPath);
	sprintf (TCL_LIBRARY_PATH, TCL_LIBRARY_FORMAT,CopyOfDefaultPath,tcl_major,tcl_minor);
	sprintf (TK_LIBRARY_PATH, TK_LIBRARY_FORMAT,CopyOfDefaultPath,tcl_major,tcl_minor);

	if (CopyOfDefaultPath) {FREE(CopyOfDefaultPath);CopyOfDefaultPath=NULL;}

	/* TCL_LIBRARY initialization */
	SetEnvironmentVariable(TCL_LIBRARY,TCL_LIBRARY_PATH);
	if (Tcl_SetVar(getTclInterp(), "tcl_library", TCL_LIBRARY_PATH, TCL_GLOBAL_ONLY) == NULL)
	{
		releaseTclInterp();
		fprintf(stderr,_("%s: An error occurred: %s\n"),"tcl_library",
			_("Impossible to set environment variable."));
		bOK = FALSE;
	}
	releaseTclInterp();

	if (Tcl_SetVar(getTclInterp(), "tclDefaultLibrary", TCL_LIBRARY_PATH, TCL_GLOBAL_ONLY) == NULL)
	{
		releaseTclInterp();
		fprintf(stderr,_("%s: An error occurred: %s\n"),"tclDefaultLibrary",
			_("Impossible to set environment variable."));
		bOK = FALSE;
	}
	releaseTclInterp();

	if (Tcl_SetVar(getTclInterp(), "tcl_pkgPath", TCL_LIBRARY_PATH, TCL_GLOBAL_ONLY) == NULL)
	{
		releaseTclInterp();
		fprintf(stderr,_("%s: An error occurred: %s\n"),"tcl_pkgPath",
			_("Impossible to set environment variable."));
		bOK = FALSE;
	}
	releaseTclInterp();

	pathPtr = Tcl_NewStringObj(TCL_LIBRARY_PATH, -1);

	/* TK_LIBRARY initialization */
	SetEnvironmentVariable(TK_LIBRARY,TK_LIBRARY_PATH);
	if (Tcl_SetVar(getTclInterp(), "tk_library", TK_LIBRARY_PATH, TCL_GLOBAL_ONLY) == NULL)
	{
		releaseTclInterp();
		fprintf(stderr,_("%s: An error occurred: %s\n"),"tk_library",
			_("Impossible to set environment variable."));
		bOK = FALSE;
	}
	releaseTclInterp();

	objPtr = Tcl_NewStringObj(TK_LIBRARY_PATH, -1);
	Tcl_ListObjAppendElement(NULL, pathPtr, objPtr);
	TclSetLibraryPath(pathPtr);

	/* encoding initialization */
	sprintf( tcl_encoding,"cp%d", GetACP() );
	if ( Tcl_SetSystemEncoding(NULL, tcl_encoding) == TCL_ERROR )
	{
		fprintf(stderr,_("%s: An error occurred: %s\n"),"Tcl_SetSystemEncoding",
			_("Impossible to set system encoding."));
		bOK = FALSE;
	}

	return bOK ;
}
/*--------------------------------------------------------------------------*/
