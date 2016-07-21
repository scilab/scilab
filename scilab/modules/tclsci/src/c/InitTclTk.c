/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005-2008 - INRIA - Allan CORNET
* Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
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


#include <string.h>
#ifndef _MSC_VER
#include <dirent.h>
#include <ctype.h>
#else
#include <windows.h>
#include "EnvTclTk.h"
#endif
#include "os_string.h"
#include "InitTclTk.h"
#include "sci_path.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "configvariable_interface.h"
#include "ScilabEval.h"
#include "TCL_Command.h"
#include "GlobalTclInterp.h"
#include "BOOL.h"
#include "PATH_MAX.h"
#include "getshortpathname.h"
/*--------------------------------------------------------------------------*/
BOOL TK_Started = FALSE;

/*--------------------------------------------------------------------------*/
static char *GetSciPath(void);
/*--------------------------------------------------------------------------*/
static void *DaemonOpenTCLsci(void* in)
/* Checks if tcl/tk has already been initialised and if not */
/* initialise it. It must find the tcl script */
{
    char *SciPath           = NULL;
    char *SciPathShort      = NULL;
    char *TkScriptpathShort = NULL;
    BOOL tkStarted          = FALSE;
    BOOL bOK                = FALSE;

    char TkScriptpath[PATH_MAX];
    char MyCommand[2048]; /* @TODO: Check for buffer overflow */


#ifndef _MSC_VER
    DIR *tmpdir = NULL;
#endif

    FILE *tmpfile2 = NULL;

    SciPath = GetSciPath();

    /* test SCI validity */
    if (SciPath == NULL)
    {
        sciprint(_("The SCI environment variable is not set.\nTCL initialisation failed !\n"));
        return (0);
    }


    SciPathShort = getshortpathname(SciPath, &bOK);
    FREE(SciPath);
    SciPath = NULL;

#ifdef TCL_MAJOR_VERSION
#ifdef TCL_MINOR_VERSION
#if TCL_MAJOR_VERSION >= 8
#if TCL_MINOR_VERSION > 0
    Tcl_FindExecutable(" ");
#endif
#endif
#endif
#endif

#ifdef _MSC_VER

    strcpy(TkScriptpath, SciPathShort);
    strcat(TkScriptpath, "/modules/tclsci/tcl/TK_Scilab.tcl");

    TkScriptpathShort = getshortpathname(TkScriptpath, &bOK);
    tmpfile2 = fopen(TkScriptpathShort, "r");
    if (tmpfile2 == NULL)
    {
        sciprint(_("Unable to find Tcl initialisation scripts.\nCheck your SCI environment variable.\nTcl initialisation failed !"));
        FREE(SciPathShort);
        SciPathShort = NULL;
        FREE(TkScriptpathShort);
        TkScriptpathShort = NULL;
        return (0);
    }
    else
    {
        fclose(tmpfile2);
    }
#else
    tmpdir = opendir(SciPathShort);
    if (tmpdir == NULL)
    {
        sciprint(_("The SCI environment variable is not set.\nTcl initialisation failed !\n"));
        FREE(SciPathShort);
        SciPathShort = NULL;
        FREE(TkScriptpathShort);
        TkScriptpathShort = NULL;
        return (0);
    }
    else
    {
        closedir(tmpdir);
    }
    strcpy(TkScriptpath, SciPathShort);
    strcat(TkScriptpath, "/modules/tclsci/tcl/TK_Scilab.tcl");
    TkScriptpathShort = getshortpathname(TkScriptpath, &bOK);
    tmpfile2 = fopen(TkScriptpathShort, "r");
    if (tmpfile2 == NULL)
    {
        sciprint(_("Unable to find Tcl initialisation scripts.\nCheck your SCI environment variable.\nTcl initialisation failed !"));
        FREE(SciPathShort);
        SciPathShort = NULL;
        FREE(TkScriptpathShort);
        TkScriptpathShort = NULL;
        return (0);
    }
    else
    {
        fclose(tmpfile2);
    }
#endif /* _MSC_VER */

    if (getTclInterp() == NULL)
    {
        releaseTclInterp();
        initTclInterp();

#ifdef _MSC_VER
        /* Initialize TCL_LIBRARY & TK_LIBRARY variables environment */
        /* Windows only */
        SetTclTkEnvironment(SciPathShort);
#endif

        if ( getTclInterp() == NULL )
        {
            Scierror(999, _("Tcl Error: Unable to create Tcl interpreter (Tcl_CreateInterp).\n"));
            FREE(SciPathShort);
            SciPathShort = NULL;
            FREE(TkScriptpathShort);
            TkScriptpathShort = NULL;
            return (0);
        }
        releaseTclInterp();

        if ( Tcl_Init(getTclInterp()) == TCL_ERROR)
        {
            releaseTclInterp();
            Scierror(999, _("Tcl Error: Error during the Tcl initialization (Tcl_Init): %s\n"), Tcl_GetStringResult(getTclInterp()));
            FREE(SciPathShort);
            SciPathShort = NULL;
            FREE(TkScriptpathShort);
            TkScriptpathShort = NULL;
            return (0);
        }
        releaseTclInterp();
        if (getenv("SCI_DISABLE_TK") == NULL)
        {
            /* When SCI_DISABLE_TK is set in the env disable the TK init
             * process. It is causing issues when Scilab is
             * used through ssh.  */
            if ( Tk_Init(getTclInterp()) == TCL_ERROR)
            {
                releaseTclInterp();
                Scierror(999, _("Tcl Error: Error during the TK initialization (Tk_Init): %s\n"), Tcl_GetStringResult(getTclInterp()));
            }
            else
            {
                tkStarted = TRUE;
            }
            releaseTclInterp();
        }


        sprintf(MyCommand, "set SciPath \"%s\";", SciPathShort);

        if ( Tcl_Eval(getTclInterp(), MyCommand) == TCL_ERROR  )
        {
            releaseTclInterp();
            Scierror(999, _("Tcl Error: Error during the Scilab/Tcl init process. Could not set SciPath: %s\n"), Tcl_GetStringResult(getTclInterp()));
            FREE(SciPathShort);
            SciPathShort = NULL;
            FREE(TkScriptpathShort);
            TkScriptpathShort = NULL;
            return (0);
        }

        releaseTclInterp();
        Tcl_CreateCommand(getTclInterp(), "ScilabEval", TCL_EvalScilabCmd, (ClientData)1, NULL);
        releaseTclInterp();
    }
    FREE(SciPathShort);
    SciPathShort = NULL;

    if (TKmainWindow == NULL && tkStarted)
    {
        TKmainWindow = Tk_MainWindow(getTclInterp());
        releaseTclInterp();
        Tk_GeometryRequest(TKmainWindow, 2, 2);
        //printf("TkScriptpathShort : |%s|\n", TkScriptpathShort);
        if ( Tcl_EvalFile(getTclInterp(), TkScriptpathShort) == TCL_ERROR  )
        {
            releaseTclInterp();
            Scierror(999, _("Tcl Error: Error during the Scilab/TK init process. Error while loading %s: %s\n"), TkScriptpathShort, Tcl_GetStringResult(getTclInterp()));
            FREE(TkScriptpathShort);
            TkScriptpathShort = NULL;
            return (0);
        }
        releaseTclInterp();
    }

    FREE(TkScriptpathShort);
    TkScriptpathShort = NULL;

    // This start a periodic and endless call to "update"
    // TCL command. This causes any TCL application to start
    // and run as if it's in the main program thread.
    startTclLoop();
    return (0);
}
/*--------------------------------------------------------------------------*/
int OpenTCLsci(void)
{
    __threadKey key;
    __InitSignalLock(&InterpReadyLock);
    __InitSignal(&InterpReady);
    // Open TCL interpreter in a separated thread.
    // Allows all Tcl application not to freeze nor decrease Scilab speed.
    // Causes also Scilab let those application live their own lifes.


    __CreateThread(&TclThread, &key, &DaemonOpenTCLsci);
    // Wait to be sure initialisation is complete.
    __LockSignal(&InterpReadyLock);
    __Wait(&InterpReady, &InterpReadyLock);
    __UnLockSignal(&InterpReadyLock);

    return 0;
}
/*--------------------------------------------------------------------------*/
BOOL CloseTCLsci(void)
{
    if ( getScilabMode() != SCILAB_NWNI )
    {
        if (isTkStarted())
        {
            setTkStarted(FALSE);
            __WaitThreadDie(TclThread);
            deleteTclInterp();
            TKmainWindow = NULL;
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
static char *GetSciPath(void)
/* force SciPath to Unix format for compatibility (Windows) */
{
    char *PathUnix = NULL;
    char *SciPathTmp = NULL;
    int i = 0;
    int lenPathUnix = 0;

    SciPathTmp = getSCI();

    if (SciPathTmp)
    {
        PathUnix = os_strdup(SciPathTmp);
        lenPathUnix = (int)strlen(PathUnix);
        for (i = 0; i < lenPathUnix; i++)
        {
            if (PathUnix[i] == '\\')
            {
                PathUnix[i] = '/';
            }
        }
    }
    if (SciPathTmp)
    {
        FREE(SciPathTmp);
        SciPathTmp = NULL;
    }
    return PathUnix;
}
/*--------------------------------------------------------------------------*/
BOOL isTkStarted(void)
{
    return TK_Started;
}
/*--------------------------------------------------------------------------*/
void setTkStarted(BOOL isTkSet)
{
    TK_Started = isTkSet;
}
/*--------------------------------------------------------------------------*/
