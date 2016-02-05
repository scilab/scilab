/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#if _MSC_VER
#include <windows.h>
#endif
#include "gw_sound.h"
#include "os_string.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
#define BEEP_ON "on"
#define BEEP_OFF "off"
/*--------------------------------------------------------------------------*/
static BOOL beepON = TRUE;
/*--------------------------------------------------------------------------*/
void doBeep(void);
/*--------------------------------------------------------------------------*/
int sci_beep(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    char *output = NULL;
    int m_out = 1, n_out = 1;

    CheckInputArgument(pvApiCtx, 0, 1);
    CheckOutputArgument(pvApiCtx, 0, 1);

    if (nbInputArgument(pvApiCtx) == 0)
    {
        if (beepON)
        {
            doBeep();
        }
    }
    else
    {
        int *piAddressVarOne = NULL;
        char *pStVarOne = NULL;
        int iType1 = 0;
        int lenStVarOne = 0;
        int m1 = 0, n1 = 0;

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

        if (iType1  != sci_strings )
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
            return 1;
        }

        sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 1;
        }

        if ( (m1 != n1) && (n1 != 1) )
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 1);
            return 1;
        }

        pStVarOne = (char*)MALLOC(sizeof(char) * (lenStVarOne + 1));
        if (pStVarOne == NULL)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 1;
        }

        if ( (strcmp(pStVarOne, BEEP_ON) == 0) || (strcmp(pStVarOne, BEEP_OFF) == 0) )
        {
            if ( strcmp(pStVarOne, BEEP_ON) == 0 )
            {
                beepON = TRUE;
            }
            else
            {
                beepON = FALSE;
            }

            FREE(pStVarOne);
            pStVarOne = NULL;
        }
        else
        {
            FREE(pStVarOne);
            pStVarOne = NULL;
            Scierror(999, _("%s: Wrong input argument #%d: '%s' or '%s' expected.\n"), fname, 1, BEEP_ON, BEEP_OFF);
            return 1;
        }
    }

    if (beepON)
    {
        output = os_strdup(BEEP_ON);
    }
    else
    {
        output = os_strdup(BEEP_OFF);
    }

    sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m_out, n_out, (const char * const*) &output);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    if (output)
    {
        FREE(output);
        output = NULL;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
void doBeep(void)
{
#ifdef _MSC_VER
    MessageBeep(0xFFFFFFFF);
#else
    puts("\a");
#endif
}
/*--------------------------------------------------------------------------*/
