/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2005 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "BOOL.h"
#include "call_scilab.h"
#include "lasterror.h"          /* clearInternalLastError, getInternalLastErrorValue */
#include "MALLOC.h"
#include "scilabmode.h"
#include "fromc.h"
#include "LaunchScilabSignal.h"
#include "localization.h"
#include "isdir.h"
#include "setgetSCIpath.h"
#include "scilabDefaults.h"
#include "tmpdir.h"
#include "inisci-c.h"
#include "scirun.h"
#include "scilabmode.h"
#include "sciquit.h"
#include "storeCommand.h"
#include "FigureList.h"
#include "../../core/src/c/TerminateCore.h"
#include "api_scilab.h"
#include "call_scilab_engine_state.h"


#ifdef _MSC_VER
#include "SetScilabEnvironmentVariables.h"
#include "getScilabDirectory.h"
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
static CALL_SCILAB_ENGINE_STATE csEngineState = CALL_SCILAB_ENGINE_STOP;

/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static void SetSciEnv(void)
{
    char *ScilabDirectory = NULL;

    ScilabDirectory = getScilabDirectory(TRUE);

    if (ScilabDirectory == NULL)
    {
        // This message must never occur, but ...
        MessageBox (NULL, "ERROR" , "Cannot determine the Scilab directory (SCI).", MB_ICONSTOP | MB_OK);
        exit(1);
    }
    SetScilabEnvironmentVariables(ScilabDirectory);

    if (ScilabDirectory)
    {
        FREE(ScilabDirectory);
        ScilabDirectory = NULL;
    }

}
#endif
/*--------------------------------------------------------------------------*/
void DisableInteractiveMode(void)
{
    setScilabMode(SCILAB_NWNI);
}

/*--------------------------------------------------------------------------*/
BOOL StartScilab(char *SCIpath, char *ScilabStartup, int Stacksize)
{
    return Call_ScilabOpen(SCIpath, TRUE, ScilabStartup, Stacksize) == 0;
}

/*--------------------------------------------------------------------------*/
/**
* Start Scilab engine
* Function created in the context of javasci v2.
* This function is just like StartScilab but provides more error messages
* in case or error. For now, it is only used in javasci v2 but it might
* be public sooner or later.
* @return
* 0: success
* -1: already running
* -2: Could not find SCI
* -3: No existing directory
* 10001: Stacksize failed (not enought memory ?).
* Any other positive integer: A Scilab internal error
*/
int Call_ScilabOpen(char *SCIpath, BOOL advancedMode, char *ScilabStartup, int Stacksize)
{
#define FORMAT_SCRIPT_STARTUP "_errorCall_ScilabOpen = exec(\"%s\", \"errcatch\", -1); exit(_errorCall_ScilabOpen);"
    char *ScilabStartupUsed = NULL;
    char *InitStringToScilab = NULL;
    int StacksizeUsed = 0;
    int lengthStringToScilab = 0;

    static int iflag = -1, ierr = 0;

    setScilabMode(SCILAB_API);
    if (advancedMode == FALSE)
    {
        DisableInteractiveMode();
    }

    if (getCallScilabEngineState() == CALL_SCILAB_ENGINE_STARTED)
        return -1;

    SetFromCToON();

    InitializeLaunchScilabSignal();

    if (SCIpath == NULL)        /* No SCIpath provided... */
    {
#ifdef _MSC_VER
        SetSciEnv();            /* Windows has a way to detect it */
#else
        /* Other doesn't */
        fprintf(stderr, "StartScilab: Could not find SCI\n");
        return -2;
#endif
    }
    else
    {
        if (!isdir(SCIpath))
        {
            /* Check if the directory actually exists */
            fprintf(stderr, "StartScilab: Could not find the directory %s\n", SCIpath);
            return -3;
        }
        else
        {
#ifdef _MSC_VER
            char env[2048];

            sprintf(env, "SCI=%s", SCIpath);
            _putenv(env);
#else
            setenv("SCI", SCIpath, 0);
#endif
            setSCIpath(SCIpath);
        }
    }

    if (ScilabStartup == NULL)
    {
        ScilabStartupUsed = strdup(DEFAULTSCILABSTARTUP);
    }
    else
    {
        ScilabStartupUsed = strdup(ScilabStartup);
    }

    if (Stacksize == 0 || Stacksize == -1)
    {
        StacksizeUsed = DEFAULTSTACKSIZE;
    }
    else
    {
        StacksizeUsed = Stacksize;
    }

    /* creates TMPDIR */
    C2F(settmpdir) ();

    /* Scilab Initialization */
    C2F(inisci) (&iflag, &StacksizeUsed, &ierr);

    if (ierr > 0)
    {
        if (ScilabStartupUsed)
        {
            FREE(ScilabStartupUsed);
            ScilabStartupUsed = NULL;
        }
        return ierr;
    }

    lengthStringToScilab = (int)(strlen(FORMAT_SCRIPT_STARTUP) + strlen(ScilabStartupUsed + 1));
    InitStringToScilab = (char *)MALLOC(lengthStringToScilab * sizeof(char));
    sprintf(InitStringToScilab, FORMAT_SCRIPT_STARTUP, ScilabStartupUsed);

    ierr = C2F(scirun) (InitStringToScilab, (long int)strlen(InitStringToScilab));

    if (ScilabStartupUsed)
    {
        FREE(ScilabStartupUsed);
        ScilabStartupUsed = NULL;
    }
    if (InitStringToScilab)
    {
        FREE(InitStringToScilab);
        InitStringToScilab = NULL;
    }

    if (ierr)
    {
        return ierr;
    }

    setCallScilabEngineState(CALL_SCILAB_ENGINE_STARTED);

    return 0;
}

/*--------------------------------------------------------------------------*/
BOOL TerminateScilab(char *ScilabQuit)
{
    if (getCallScilabEngineState() == CALL_SCILAB_ENGINE_STARTED)
    {
        if (getScilabMode() != SCILAB_NWNI)
        {
            ExitScilab();
        }
        else
        {
            TerminateCorePart2();
        }

        /* Make sure that the error management is reset. See bug #8830 */
        clearInternalLastError();

        ReleaseLaunchScilabSignal();
        setCallScilabEngineState(CALL_SCILAB_ENGINE_STOP);

        /* restore default mode */
        setScilabMode(SCILAB_API);

        return TRUE;
    }
    return FALSE;
}

/*--------------------------------------------------------------------------*/
/**
* function called javasci
*/
void ScilabDoOneEvent(void)
{
    if (getCallScilabEngineState() == CALL_SCILAB_ENGINE_STARTED)
    {
        if (getScilabMode() != SCILAB_NWNI)
        {
            C2F(scirun) ("quit;", (int)strlen("quit;"));
        }
    }
}

/*--------------------------------------------------------------------------*/
int ScilabHaveAGraph(void)
{
    if (getCallScilabEngineState() == CALL_SCILAB_ENGINE_STARTED)
    {
        return sciHasFigures();
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
CALL_SCILAB_ENGINE_STATE setCallScilabEngineState(CALL_SCILAB_ENGINE_STATE state)
{
    csEngineState = state;
    return csEngineState;
}

/*--------------------------------------------------------------------------*/
CALL_SCILAB_ENGINE_STATE getCallScilabEngineState(void)
{
    return csEngineState;
}

/*--------------------------------------------------------------------------*/
sci_types getVariableType(char *varName)
{
    int iSciType = -1;
    SciErr sciErr = getNamedVarType(pvApiCtx, (char *)varName, &iSciType);

    if (sciErr.iErr == API_ERROR_NAMED_UNDEFINED_VAR)
    {
        return -2;
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return (sci_types) iSciType;
}

/*--------------------------------------------------------------------------*/

/**
 * Call the Scilab function getLastErrorMessage
 * Take the result (a matrix of string) and concatenate into a single string
 * This is way easier to manage in swig.
*/
char *getLastErrorMessageSingle(void)
{
    int iNbLines, i, nbChar = 0;
    const char **msgs = getInternalLastErrorMessage(&iNbLines);
    char *concat;

    for (i = 0; i < iNbLines; i++)
    {
        nbChar += (int)strlen(msgs[i]);
    }
    concat = (char *)malloc((nbChar + 1) * sizeof(char));
    strcpy(concat, "");
    for (i = 0; i < iNbLines; i++)
    {
        strcat(concat, msgs[i]);
    }
    return concat;
}

/*--------------------------------------------------------------------------*/
 int getLastErrorValue(void)
 {
     /* defined in lasterror.h */
     return getInternalLastErrorValue();
 }
 /*--------------------------------------------------------------------------*/
