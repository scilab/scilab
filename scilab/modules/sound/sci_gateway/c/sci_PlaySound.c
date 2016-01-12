/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2007 - INRIA - Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <wchar.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <Windows.h>
#pragma comment(lib, "winmm.lib")
#endif
#include "gw_sound.h"
#include "expandPathVariable.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
static int playsound(const char* cFilename);
/*--------------------------------------------------------------------------*/
/* private function called by playsnd */
/*--------------------------------------------------------------------------*/
int sci_PlaySound(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    char* pStVarOne = NULL;
    int iType1 = 0;
    int lenStVarOne = 0;
    int m1 = 0, n1 = 0;
    char* expandedPath = NULL;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    if (iType1 != sci_strings )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 1;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &pStVarOne))
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    expandedPath = expandPathVariable(pStVarOne);
    if (pStVarOne)
    {
        freeAllocatedSingleString(pStVarOne);
        pStVarOne = NULL;
    }

#ifdef _MSC_VER
    {
        if (expandedPath)
        {
            playsound(expandedPath);
            FREE(expandedPath);
            expandedPath = NULL;
        }

        AssignOutputVariable(pvApiCtx, 1) = 0;
        ReturnArguments(pvApiCtx);
    }
#else
    {
        if (expandedPath)
        {
            FREE(expandedPath);
            expandedPath = NULL;
        }
        Scierror(999, _("%s: An error occurred: %s\n"), fname, _("Cannot play file.") );
    }
#endif
    return 0;
}
/*--------------------------------------------------------------------------*/
static int playsound(const char* cFilename)
{
#ifdef _MSC_VER
    if (cFilename)
    {
        /* Stop Playing*/
        PlaySoundA(NULL, NULL, SND_PURGE);
        PlaySoundA(cFilename, NULL, SND_ASYNC | SND_FILENAME);
    }
#endif
    return 0;
}
/*--------------------------------------------------------------------------*/

