/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "gw_functions.h"
#include "api_scilab.h"
#include "machine.h"
#include "recursionFunction.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intexecstr)(); /* fortran */
/*--------------------------------------------------------------------------*/
#define ERRCATCH_KEYWORD "errcatch"
#define MESSAGE_KEYWORD "m"
#define MESSAGE_DEFAULT_KEYWORD "n"
/*--------------------------------------------------------------------------*/
int C2F(sci_execstr)(char *fname, unsigned long fname_len)
{
    if ( isRecursionCallToFunction() )
    {
        C2F(intexecstr)(fname, fname_len);
    }
    else
    {
        SciErr sciErr;

        int *piAddressVarOne = NULL;
        int iType1 = 0;

        CheckRhs(1, 3);
        CheckLhs(0, 1);

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

        /* execstr([])*/
        if (iType1 == sci_matrix)
        {
            int m1 = 0, n1 = 0;

            sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            if ((m1 == n1) && (m1 == 0)) /* [] */
            {
                sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }

                LhsVar(1) = Rhs + 1;

                PutLhsVar();
                return 0;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
                return 0;
            }
        }

        if (iType1 != sci_strings)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
            return 0;
        }

        if (Rhs > 1)
        {
            int m2 = 0, n2 = 0;
            int *piAddressVarTwo = NULL;
            char *pStVarTwo = NULL;
            int lenStVarTwo = 0;
            int iType2 = 0;

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

            if (iType2 != sci_strings)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
                return 0;
            }

            sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, NULL);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 0;
            }

            if (m2 * n2 != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
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

                if (strcmp(pStVarTwo, ERRCATCH_KEYWORD))
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: 'errcatch' expected.\n"), fname, 2);
                    FREE(pStVarTwo);
                    pStVarTwo = NULL;
                    return 0;
                }

                FREE(pStVarTwo);
                pStVarTwo = NULL;
            }
            else
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }

        if (Rhs > 2)
        {
            int m3 = 0, n3 = 0;
            int *piAddressVarThree = NULL;
            char *pStVarThree = NULL;
            int lenStVarThree = 0;
            int iType3 = 0;

            sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                return 0;
            }

            sciErr = getVarType(pvApiCtx, piAddressVarThree, &iType3);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                return 0;
            }

            if (iType3 != sci_strings)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
                return 0;
            }

            sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &m3, &n3, &lenStVarThree, NULL);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                return 0;
            }

            if (m3 * n3 != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 3);
                return 0;
            }

            pStVarThree = (char*)MALLOC(sizeof(char) * (lenStVarThree + 1));
            if (pStVarThree)
            {
                sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &m3, &n3, &lenStVarThree, &pStVarThree);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 0;
                }

                if ( strcmp(pStVarThree, MESSAGE_KEYWORD) && strcmp(pStVarThree, MESSAGE_DEFAULT_KEYWORD) )
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: 'm' or 'n' expected.\n"), fname, 3);
                    FREE(pStVarThree);
                    pStVarThree = NULL;
                    return 0;
                }

                FREE(pStVarThree);
                pStVarThree = NULL;
            }
            else
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }

        C2F(intexecstr)(fname, fname_len);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
