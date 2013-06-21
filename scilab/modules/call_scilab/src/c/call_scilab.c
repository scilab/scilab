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
#include "BOOL.h"
#include "call_scilab.h"
#include "lasterror.h"          /* clearInternalLastError, getInternalLastErrorValue */
#include "MALLOC.h"
#include "sci_mode.h"
#include "fromc.h"
#include "LaunchScilabSignal.h"
#include "isdir.h"
#include "sci_path.h"
#include "scilabDefaults.h"
#include "sci_tmpdir.h"
#include "scirun.h"
#include "sciquit.h"
#include "storeCommand.h"
#include "FigureList.h"
#include "../../core/src/c/TerminateCore.h"
#include "api_scilab.h"
#include "call_scilab_engine_state.h"
#include "os_strdup.h"
#include "charEncoding.h"
#include "InitScilab.h"
#include "scilabRead.h"
#include "scilabWrite.hxx"

#ifdef _MSC_VER
#include "SetScilabEnvironmentVariables.h"
#include "getScilabDirectory.h"
#include <Windows.h>
#endif

extern char *getCmdLine(void);

static void TermPrintf(char *text)
{
    //std::cout << text;
    printf("%s", text);
}



/*--------------------------------------------------------------------------*/
static CALL_SCILAB_ENGINE_STATE csEngineState = CALL_SCILAB_ENGINE_STOP;
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

#define FORMAT_SCRIPT_STARTUP "_errorCall_ScilabOpen = exec(\"%s\", \"errcatch\", -1); exit(_errorCall_ScilabOpen);"

int Call_ScilabOpen(char *SCIpath, BOOL advancedMode, char *ScilabStartup, int Stacksize)
{
    char *InitStringToScilab = NULL;
    static int iflag = -1, ierr = 0;
    ScilabEngineInfo* pSEI = NULL;

    setScilabMode(SCILAB_API);
    if (advancedMode == FALSE)
    {
        DisableInteractiveMode();
    }

    if (getCallScilabEngineState() == CALL_SCILAB_ENGINE_STARTED)
    {
        return -1;
    }

    SetFromCToON();

    if (SCIpath == NULL)        /* No SCIpath provided... */
    {
#ifndef _MSC_VER
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
    }

    pSEI = InitScilabEngineInfo();
    if (ScilabStartup)
    {
        int lengthStringToScilab = (int)(strlen(FORMAT_SCRIPT_STARTUP) + strlen(ScilabStartup) + 1);
        InitStringToScilab = (char *)MALLOC(lengthStringToScilab * sizeof(char));
        sprintf(InitStringToScilab, FORMAT_SCRIPT_STARTUP, ScilabStartup);
        pSEI->iNoStart = 1;
    }

    setScilabInputMethod(&getCmdLine);
    setScilabOutputMethod(&TermPrintf);

    /* Scilab Initialization */
    pSEI->pstFile = InitStringToScilab;
    pSEI->iNoJvm = 1;
    pSEI->iConsoleMode = 1;
    ierr = StartScilabEngine(pSEI);
    FREE(pSEI);

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
        ScilabEngineInfo* pSEI = InitScilabEngineInfo();
        pSEI->pstFile = ScilabQuit;
        StopScilabEngine(pSEI);

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
#if 0
            C2F(scirun) ("quit;", (int)strlen("quit;"));
#endif
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
    SciErr sciErr = getNamedVarType(NULL, (char*)varName, &iSciType);

    if (sciErr.iErr == API_ERROR_NAMED_UNDEFINED_VAR)
    {
        return (sci_types) - 2;
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return (sci_types) - 1;
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
    return wide_string_to_UTF8(getLastErrorMessage());
}

/*--------------------------------------------------------------------------*/
int getLastErrorValue(void)
{
    /* defined in lasterror.h */
    // FIXME : Call system_env function
    //return getInternalLastErrorValue();
    return 0;
}
/*--------------------------------------------------------------------------*/
