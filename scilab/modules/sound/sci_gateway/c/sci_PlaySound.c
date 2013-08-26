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
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
static int playsound(wchar_t *wcFilename);
/*--------------------------------------------------------------------------*/
/* private function called by playsnd */
/*--------------------------------------------------------------------------*/
int sci_Playsound (char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    wchar_t *pStVarOne = NULL;
    int iType1 = 0;
    int lenStVarOne = 0;
    int m1 = 0, n1 = 0;
    wchar_t *expandedPath = NULL;

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
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 1;
    }

    if (getAllocatedSingleWideString(pvApiCtx, piAddressVarOne, &pStVarOne))
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    //    if ( (m1 != n1) && (n1 != 1) )
    //    {
    //        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
    //        return 1;
    //    }
    //
    //    sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
    //    if (sciErr.iErr)
    //    {
    //        printError(&sciErr, 0);
    //        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
    //        return 1;
    //    }
    //
    //    pStVarOne = (wchar_t*)MALLOC(sizeof(wchar_t) * (lenStVarOne + 1));
    //    if (pStVarOne == NULL)
    //    {
    //        Scierror(999, _("%s: Memory allocation error.\n"), fname);
    //        return 1;
    //    }
    //
    //    sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
    //    if (sciErr.iErr)
    //    {
    //        printError(&sciErr, 0);
    //        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
    //        return 1;
    //    }

    expandedPath = expandPathVariableW(pStVarOne);
    if (pStVarOne)
    {
        FREE(pStVarOne);
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
static int playsound(wchar_t *wcFilename)
{
#ifdef _MSC_VER
    if (wcFilename)
    {
        /* Stop Playing*/
        PlaySoundW(NULL, NULL, SND_PURGE);
        PlaySoundW(wcFilename, NULL, SND_ASYNC | SND_FILENAME);
    }
#endif
    return 0;
}
/*--------------------------------------------------------------------------*/

