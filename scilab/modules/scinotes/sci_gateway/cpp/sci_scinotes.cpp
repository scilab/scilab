/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - Calixte DENIZET
 * Copyright (C) 2010 - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "SciNotes.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include <wchar.h>
#include "callscinotes.h"
#include "gw_scinotes.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
}

/*--------------------------------------------------------------------------*/
int sci_scinotes(char * fname, void* pvApiCtx)
{
    SciErr sciErr;

    CheckRhs(0, 3);
    CheckLhs(0, 1);

    if (Rhs == 0)
    {
        try
        {
            callSciNotesW(NULL, 0);
        }
        catch (GiwsException::JniCallMethodException exception)
        {
            Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
        }
        catch (GiwsException::JniException exception)
        {
            Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
        }
    }
    else
    {
        int m1 = 0, n1 = 0;
        int *piAddressVarOne = NULL;
        char **pStVarOne = NULL;
        int i = 0;
        int iType1 = 0;
        char *functionName = NULL;

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

        if (iType1 != sci_strings)
        {
            Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &pStVarOne))
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (Rhs >= 2)           //get line numbers
        {
            int *piAddressVarTwo = NULL;
            int m2 = 0, n2 = 0;
            double *pdblVarTwo = NULL;
            int iType2 = 0;

            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                freeAllocatedMatrixOfString(m1, n1, pStVarOne);
                return 0;
            }

            sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                freeAllocatedMatrixOfString(m1, n1, pStVarOne);
                return 0;
            }

            if (iType2 != sci_matrix && iType2 != sci_strings)
            {
                Scierror(999, _("%s: Wrong type for argument #%d: Real matrix or \'readonly\' expected.\n"), fname, 2);
                freeAllocatedMatrixOfString(m1, n1, pStVarOne);
                return 0;
            }

            if (iType2 == sci_strings)
            {
                /* get dimensions */
                char** pStVarTwo = NULL;

                if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarTwo, &m2, &n2, &pStVarTwo))
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                    freeAllocatedMatrixOfString(m1, n1, pStVarOne);
                    return 0;
                }

                try
                {
                    callSciNotesWWithOption(pStVarOne, pStVarTwo, m2 * n2, m1 * n1);
                }
                catch (GiwsException::JniCallMethodException exception)
                {
                    Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
                    freeAllocatedMatrixOfString(m1, n1, pStVarOne);
                    freeAllocatedMatrixOfString(m2, n2, pStVarTwo);
                    return 0;
                }
                catch (GiwsException::JniException exception)
                {
                    Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
                    freeAllocatedMatrixOfString(m1, n1, pStVarOne);
                    freeAllocatedMatrixOfString(m2, n2, pStVarTwo);
                    return 0;
                }
                freeAllocatedMatrixOfString(m2, n2, pStVarTwo);
            }
            else
            {
                if (isVarComplex(pvApiCtx, piAddressVarTwo) == 1)
                {
                    Scierror(999, _("%s: Wrong type for argument #%d: Real matrix expected.\n"), fname, 2);
                    freeAllocatedMatrixOfString(m1, n1, pStVarOne);
                    return 0;
                }

                sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &pdblVarTwo);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                    freeAllocatedMatrixOfString(m1, n1, pStVarOne);
                    return 0;
                }

                if (m2 * n2 != m1 * n1)
                {
                    Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Same dimensions expected.\n"), fname, 1, 2);
                    freeAllocatedMatrixOfString(m1, n1, pStVarOne);
                    return 0;
                }

                if (Rhs == 3)
                {
                    int *piAddressVarThree = NULL;

                    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                        return 0;
                    }

                    if (!isStringType(pvApiCtx, piAddressVarThree))
                    {
                        Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
                        freeAllocatedMatrixOfString(m1, n1, pStVarOne);
                        return 0;
                    }

                    int ret = getAllocatedSingleString(pvApiCtx, piAddressVarThree, &functionName);

                    if (ret)
                    {
                        Scierror(999, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
                        freeAllocatedMatrixOfString(m1, n1, pStVarOne);
                        return 0;
                    }
                }

                try
                {
                    callSciNotesWWithLineNumberAndFunction(pStVarOne, pdblVarTwo, functionName, m1 * n1);
                }
                catch (GiwsException::JniCallMethodException exception)
                {
                    Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
                }
                catch (GiwsException::JniException exception)
                {
                    Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
                }
            }
        }
        else
        {
            try
            {
                callSciNotesW(pStVarOne, m1 * n1);
            }
            catch (GiwsException::JniCallMethodException exception)
            {
                Scierror(999, "%s: %s\n", fname, exception.getJavaDescription().c_str());
            }
            catch (GiwsException::JniException exception)
            {
                Scierror(999, "%s: %s\n", fname, exception.whatStr().c_str());
            }
        }

        freeAllocatedMatrixOfString(m1, n1, pStVarOne);
        if (functionName)
        {
            freeAllocatedSingleString(functionName);
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
