/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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

extern "C"
{
#include "gw_windows_tools.h"
#include "PATH_MAX.h"
#include "sci_malloc.h" /* MALLOC */
#include "spawncommand.h"
#include "sciprint.h"
#include "sci_tmpdir.h"
#include "FileExist.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "getshortpathname.h"
#include "api_scilab.h"
#include "charEncoding.h"
}
/*--------------------------------------------------------------------------*/
#define BUFSIZE 4096
/*--------------------------------------------------------------------------*/
static int PrintOuput(char **output, int nbrlines);
/*--------------------------------------------------------------------------*/
int sci_dos(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int iType1	= 0;
    int m1 = 0, n1 = 0;
    wchar_t *pStVarOne = NULL;
    int lenStVarOne = 0;

    char **Output = NULL;
    int numberoflines = 0;
    BOOL ECHOMODE = FALSE;

    CheckRhs(1, 2);
    CheckLhs(1, 3);

    if (Rhs == 2)
    {
        int *piAddressVarTwo = NULL;
        int m2 = 0, n2 = 0;
        int iType2 = 0;
        char *pStVarTwo = NULL;
        int lenStVarTwo = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (iType2  != sci_strings )
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
            return 0;
        }

        sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if ( (m2 != n2) && (n2 != 1) )
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 2);
            return 0;
        }

        pStVarTwo = (char*)MALLOC(sizeof(char) * (lenStVarTwo + 1));
        if (pStVarTwo)
        {
            sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &pStVarTwo);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 0;
            }

            if ( strcmp(pStVarTwo, "-echo") )
            {
                FREE(pStVarTwo);
                pStVarTwo = NULL;
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' expected.\n"), fname, 2, "-echo");
                return 0;
            }
            else
            {
                ECHOMODE = TRUE;
            }
        }
        else
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (iType1  != sci_strings )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if ( (m1 != n1) && (n1 != 1) )
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 1);
        return 0;
    }

    pStVarOne = (wchar_t*)MALLOC(sizeof(wchar_t) * (lenStVarOne + 1));
    if (pStVarOne)
    {
        double exitCode = 0.;
        BOOL DetachProcessOption = FALSE;
        BOOL *StatusExit = NULL;

        sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        DetachProcessOption = DetectDetachProcessInCommandLine(pStVarOne);
        exitCode = (double)spawncommand(pStVarOne, DetachProcessOption);
        FREE(pStVarOne);

        StatusExit = (BOOL*)MALLOC(sizeof(BOOL));

        if (DetachProcessOption)
        {
            if ( strlen((const char *)(pipeSpawnErr.OutputBuffer)) )
            {
                /* StdErr will be "Output" */
                *StatusExit = FALSE;
                Output = CreateOuput(&pipeSpawnErr, DetachProcessOption);
                numberoflines = pipeSpawnErr.NumberOfLines;
            }
            else
            {
                /* StdOut will be "Output" */
                *StatusExit = TRUE;
                Output = CreateOuput(&pipeSpawnOut, DetachProcessOption);
                numberoflines = pipeSpawnOut.NumberOfLines;
            }
        }
        else
        {
            char FileTMPDir[PATH_MAX + 16];
            BOOL bConvert = FALSE;

            char *TMPDirLong = getTMPDIR();
            char *TMPDirShort = getshortpathname(TMPDirLong, &bConvert);

            sprintf(FileTMPDir, "%s\\DOS.OK", TMPDirLong);
            FREE(TMPDirLong);
            TMPDirLong = NULL;
            FREE(TMPDirShort);
            TMPDirShort = NULL;

            if (FileExist(FileTMPDir))
            {
                DeleteFile(FileTMPDir);
                /* StdOut will be "Output" */
                *StatusExit = TRUE;
                Output = CreateOuput(&pipeSpawnOut, DetachProcessOption);
                numberoflines = pipeSpawnOut.NumberOfLines;
            }
            else
            {
                /* StdErr will be "Output" */
                *StatusExit = FALSE;
                Output = CreateOuput(&pipeSpawnErr, DetachProcessOption);
                numberoflines = pipeSpawnErr.NumberOfLines;
            }
        }

        if (ECHOMODE)
        {
            PrintOuput(Output, numberoflines);
        }

        if (Lhs == 1)
        {
            int m_out = 1, n_out = 1;
            sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, m_out, n_out, StatusExit);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(1) = Rhs + 1;
        }
        else
        {
            int m_out2 = 1;
            int n_out2 = 1;

            if (Output && Output[0])
            {
                int m_out1 = numberoflines;
                int n_out1 = 1;
                sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, m_out1, n_out1, Output);
            }
            else
            {
                /* returns [] */
                int m_out1 = 0;
                int n_out1 = 0;
                sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out1, n_out1, NULL);
            }

            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(1) = Rhs + 1;

            sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 2, m_out2, n_out2, StatusExit);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(2) = Rhs + 2;
        }

        if (Lhs > 2)
        {
            int m_out3 = 1, n_out3 = 1;
            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 3, m_out3, n_out3, &exitCode);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(3) = Rhs + 3;
        }
        if (StatusExit)
        {
            FREE(StatusExit);
            StatusExit = NULL;
        }
        freeArrayOfString(Output, numberoflines);

        ClosePipeInfo (pipeSpawnOut);
        ClosePipeInfo (pipeSpawnErr);

        PutLhsVar();
    }
    else
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
static int PrintOuput(char **output, int nbrlines)
{
    if (output)
    {
        int i = 0;
        for (i = 0; i < nbrlines; i++)
        {
            if (output[i])
            {
                sciprint("%s\n", output[i]);
            }
        }
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/

