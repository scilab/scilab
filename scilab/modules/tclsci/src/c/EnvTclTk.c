/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#include <windows.h>
#include <stdio.h>
#include <tcl.h>
#include <tk.h>

#include "PATH_MAX.h"
#include "sci_malloc.h"
#include "EnvTclTk.h"
#include "setenvtcl.h"
#include "GlobalTclInterp.h"
#include "localization.h"
#include "ConvertSlash.h"
#include "configvariable_interface.h"
/*--------------------------------------------------------------------------*/
extern void	TclSetLibraryPath(Tcl_Obj * pathPtr);
/*--------------------------------------------------------------------------*/
#if defined(TCL_MAJOR_VERSION) && defined(TCL_MAJOR_VERSION)
#if TCL_MAJOR_VERSION == 8
#if TCL_MINOR_VERSION == 4
static CONST char * Tcl_GetEncodingNameFromEnvironment(Tcl_DString *bufPtr);
#endif
#endif
#endif
/*--------------------------------------------------------------------------*/
BOOL SetTclTkEnvironment(char *DefaultPath)
{
#define TCL_LIBRARY "TCL_LIBRARY"
#define TCL_LIBRARY_FORMAT "%s/modules/tclsci/tcl/tcl%d.%d"

#define TK_LIBRARY "TK_LIBRARY"
#define TK_LIBRARY_FORMAT "%s/modules/tclsci/tcl/tk%d.%d"

#define TCL_DEFAULT_ENCODING_DIR_FORMAT "%s/modules/tclsci/tcl/tcl%d.%d/encoding"

    int tcl_major = 8;
    int tcl_minor = 4; /* default */
    int tcl_patchLevel = 0;
    int tcl_type = 0;

    BOOL bOK = TRUE;

    char TCL_LIBRARY_PATH[PATH_MAX];
    char TK_LIBRARY_PATH[PATH_MAX];
    char TCL_DEFAULT_ENCODING_DIR[PATH_MAX];

    Tcl_DString encodingName;

    char ShortPath[PATH_MAX];
    char *CopyOfDefaultPath = NULL;

    Tcl_Obj *pathPtr = NULL;
    Tcl_Obj *objPtr = NULL;

    CopyOfDefaultPath = MALLOC(((int)strlen(DefaultPath) + 1) * sizeof(char));
    if (CopyOfDefaultPath == NULL)
    {
        return FALSE;
    }

    if (getScilabMode() == SCILAB_STD)
    {
        /* redirect stdout, stderr in console */
        freopen("CONOUT$", "wb", stdout); /* redirect stdout --> CONOUT$*/
        freopen("CONOUT$", "wb", stderr); /* redirect stderr --> CONOUT$*/
    }

    Tcl_GetVersion(&tcl_major, &tcl_minor, &tcl_patchLevel, &tcl_type);

    GetShortPathName(DefaultPath, ShortPath, PATH_MAX);
    AntislashToSlash(ShortPath, CopyOfDefaultPath);
    sprintf (TCL_LIBRARY_PATH, TCL_LIBRARY_FORMAT, CopyOfDefaultPath, tcl_major, tcl_minor);
    sprintf (TK_LIBRARY_PATH, TK_LIBRARY_FORMAT, CopyOfDefaultPath, tcl_major, tcl_minor);
    sprintf (TCL_DEFAULT_ENCODING_DIR,
             TCL_DEFAULT_ENCODING_DIR_FORMAT,
             CopyOfDefaultPath,
             tcl_major,
             tcl_minor);

    if (CopyOfDefaultPath)
    {
        FREE(CopyOfDefaultPath);
        CopyOfDefaultPath = NULL;
    }

    /* TCL_LIBRARY initialization */
    SetEnvironmentVariable(TCL_LIBRARY, TCL_LIBRARY_PATH);
    setenvtcl(TCL_LIBRARY, TCL_LIBRARY_PATH);
    if (Tcl_SetVar(getTclInterp(), "tcl_library", TCL_LIBRARY_PATH, TCL_GLOBAL_ONLY) == NULL)
    {
        releaseTclInterp();
        fprintf(stderr, _("%s: An error occurred: %s\n"), "tcl_library",
                _("Impossible to set environment variable."));
        bOK = FALSE;
    }
    releaseTclInterp();

    if (Tcl_SetVar(getTclInterp(), "tclDefaultLibrary", TCL_LIBRARY_PATH, TCL_GLOBAL_ONLY) == NULL)
    {
        releaseTclInterp();
        fprintf(stderr, _("%s: An error occurred: %s\n"), "tclDefaultLibrary",
                _("Impossible to set environment variable."));
        bOK = FALSE;
    }
    releaseTclInterp();

    if (Tcl_SetVar(getTclInterp(), "tcl_pkgPath", TCL_LIBRARY_PATH, TCL_GLOBAL_ONLY) == NULL)
    {
        releaseTclInterp();
        fprintf(stderr, _("%s: An error occurred: %s\n"), "tcl_pkgPath",
                _("Impossible to set environment variable."));
        bOK = FALSE;
    }
    releaseTclInterp();

    pathPtr = Tcl_NewStringObj(TCL_LIBRARY_PATH, -1);

    /* TK_LIBRARY initialization */
    SetEnvironmentVariable(TK_LIBRARY, TK_LIBRARY_PATH);
    setenvtcl(TK_LIBRARY, TK_LIBRARY_PATH);
    if (Tcl_SetVar(getTclInterp(), "tk_library", TK_LIBRARY_PATH, TCL_GLOBAL_ONLY) == NULL)
    {
        releaseTclInterp();
        fprintf(stderr, _("%s: An error occurred: %s\n"), "tk_library",
                _("Impossible to set environment variable."));
        bOK = FALSE;
    }
    releaseTclInterp();

    objPtr = Tcl_NewStringObj(TK_LIBRARY_PATH, -1);
    Tcl_ListObjAppendElement(NULL, pathPtr, objPtr);
    TclSetLibraryPath(pathPtr);

    /* encoding initialization */
    Tcl_SetDefaultEncodingDir(TCL_DEFAULT_ENCODING_DIR);
    if ( Tcl_SetSystemEncoding(NULL, Tcl_GetEncodingNameFromEnvironment(&encodingName)) == TCL_ERROR )
    {
        fprintf(stderr, _("%s: An error occurred: %s\n"), "Tcl_SetSystemEncoding",
                _("Impossible to set system encoding."));
        bOK = FALSE;
    }
    Tcl_DStringFree(&encodingName);

    return bOK ;
}
/*--------------------------------------------------------------------------*/
#if defined(TCL_MAJOR_VERSION) && defined(TCL_MAJOR_VERSION)
#if TCL_MAJOR_VERSION == 8
#if TCL_MINOR_VERSION == 4
static CONST char * Tcl_GetEncodingNameFromEnvironment(Tcl_DString *bufPtr)
{
    Tcl_DStringInit(bufPtr);
    wsprintfA(Tcl_DStringValue(bufPtr), "cp%d", GetACP());
    return Tcl_DStringValue(bufPtr);
}
#endif
#endif
#endif
/*--------------------------------------------------------------------------*/
