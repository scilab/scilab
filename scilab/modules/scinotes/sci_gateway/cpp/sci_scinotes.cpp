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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SciNotes.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include <wchar.h>
#include "callscinotes.h"
#include "gw_scinotes.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
}

/*--------------------------------------------------------------------------*/
int
sci_scinotes(char *fname, unsigned long fname_len)
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
            Scierror(999, "%s: %s\n", fname,
                    exception.getJavaDescription().c_str());
        }
        catch (GiwsException::JniException exception)
        {
            Scierror(999, "%s: %s\n", fname, exception.what());
        }
    }
    else
    {
        int m1 = 0, n1 = 0;
        int *piAddressVarOne = NULL;
        wchar_t **pStVarOne = NULL;
        int *lenStVarOne = NULL;
        int i = 0;
        int iType1 = 0;
        char *functionName = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (iType1 != sci_strings)
        {
            Scierror(
                    999,
                    _("%s: Wrong type for argument %d: String matrix expected.\n"),
                    fname, 1);
            return 0;
        }

        /* get dimensions */
        sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1,
                lenStVarOne, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        lenStVarOne = (int*) MALLOC(sizeof(int) * (m1 * n1));
        if (lenStVarOne == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        /* get lengths */
        sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1,
                lenStVarOne, pStVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            FREE(lenStVarOne);
            return 0;
        }

        pStVarOne = (wchar_t **) MALLOC(sizeof(wchar_t *) * (m1 * n1));
        if (pStVarOne == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            FREE(lenStVarOne);
            return 0;
        }

        for (i = 0; i < m1 * n1; i++)
        {
            pStVarOne[i] = (wchar_t *) MALLOC(sizeof(wchar_t) * (lenStVarOne[i]
                    + 1));
            if (pStVarOne[i] == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                for (; i >= 0; i--)
                {
                    FREE(pStVarOne[i]);
                }
                FREE(lenStVarOne);
                return 0;
            }
        }

        /* get strings */
        sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1,
                lenStVarOne, pStVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            freeArrayOfWideString(pStVarOne, m1 * n1);
            FREE(lenStVarOne);
            return 0;
        }

        if (Rhs >= 2) //get line numbers
        {
            int* piAddressVarTwo = NULL;
            int m2 = 0, n2 = 0;
            double* pdblVarTwo = NULL;
            int iType2 = 0;

            sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeArrayOfWideString(pStVarOne, m1 * n1);
                FREE(lenStVarOne);
                return 0;
            }

            sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeArrayOfWideString(pStVarOne, m1 * n1);
                FREE(lenStVarOne);
                return 0;
            }

            if (iType2 != sci_matrix && iType2 != sci_strings)
            {
                Scierror(
                        999,
                        _("%s: Wrong type for argument %d: Real matrix or \'readonly\' expected.\n"),
                        fname, 2);
                freeArrayOfWideString(pStVarOne, m1 * n1);
                FREE(lenStVarOne);
                return 0;
            }

            if (iType2 == sci_strings)
            {
                /* get dimensions */
                wchar_t **pStVarTwo = NULL;
                int *lenStVarTwo = NULL;

                sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarTwo, &m2,
                        &n2, lenStVarTwo, NULL);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    freeArrayOfWideString(pStVarOne, m1 * n1);
                    FREE(lenStVarOne);
                    return 0;
                }

                if (m2 != 1 || n2 != 1)
                {
                    Scierror(
                            999,
                            _("%s: Wrong type for argument %d: Real matrix or \'readonly\' expected.\n"),
                            fname, 2);
                    freeArrayOfWideString(pStVarOne, m1 * n1);
                    FREE(lenStVarOne);
                    return 0;
                }

                lenStVarTwo = (int*) MALLOC(sizeof(int));
                if (lenStVarTwo == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    freeArrayOfWideString(pStVarOne, m1 * n1);
                    FREE(lenStVarOne);
                    return 0;
                }

                /* get lengths */
                sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarTwo, &m2,
                        &n2, lenStVarTwo, pStVarTwo);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(lenStVarTwo);
                    freeArrayOfWideString(pStVarOne, m1 * n1);
                    FREE(lenStVarOne);
                    return 0;
                }

                pStVarTwo = (wchar_t **) MALLOC(sizeof(wchar_t *));
                if (pStVarTwo == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    FREE(lenStVarTwo);
                    freeArrayOfWideString(pStVarOne, m1 * n1);
                    FREE(lenStVarOne);
                    return 0;
                }

                pStVarTwo[0] = (wchar_t *) MALLOC(sizeof(wchar_t)
                        * (lenStVarTwo[0] + 1));
                if (pStVarTwo[0] == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    FREE(pStVarTwo);
                    FREE(lenStVarTwo);
                    freeArrayOfWideString(pStVarOne, m1 * n1);
                    FREE(lenStVarOne);
                    return 0;
                }

                /* get strings */
                sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarTwo, &m2,
                        &n2, lenStVarTwo, pStVarTwo);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(pStVarTwo);
                    FREE(lenStVarTwo);
                    freeArrayOfWideString(pStVarOne, m1 * n1);
                    FREE(lenStVarOne);
                    return 0;
                }

                try
                {
                    callSciNotesWWithOption(pStVarOne, pStVarTwo, m1 * n1);
                }
                catch (GiwsException::JniCallMethodException exception)
                {
                    Scierror(999, "%s: %s\n", fname,
                            exception.getJavaDescription().c_str());
                    FREE(pStVarTwo);
                    FREE(lenStVarTwo);
                    freeArrayOfWideString(pStVarOne, m1 * n1);
                    FREE(lenStVarOne);
                    return 0;
                }
                catch (GiwsException::JniException exception)
                {
                    Scierror(999, "%s: %s\n", fname, exception.what());
                    FREE(pStVarTwo);
                    FREE(lenStVarTwo);
                    freeArrayOfWideString(pStVarOne, m1 * n1);
                    FREE(lenStVarOne);
                    return 0;
                }
                freeArrayOfWideString(pStVarTwo, 1);
                FREE(lenStVarTwo);
            }
            else
            {
                if (isVarComplex(pvApiCtx, piAddressVarTwo) == 1)
                {
                    Scierror(
                            999,
                            _("%s: Wrong type for argument %d: Real matrix expected.\n"),
                            fname, 2);
                    freeArrayOfWideString(pStVarOne, m1 * n1);
                    FREE(lenStVarOne);
                    return 0;
                }

                sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2,
                        &pdblVarTwo);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    freeArrayOfWideString(pStVarOne, m1 * n1);
                    FREE(lenStVarOne);
                    return 0;
                }

                if (m2 * n2 != m1 * n1)
                {
                    Scierror(
                            999,
                            _("%s: Wrong size for input arguments #%d and #%d: Same dimensions expected.\n"),
                            fname, 1, 2);
                    freeArrayOfWideString(pStVarOne, m1 * n1);
                    FREE(lenStVarOne);
                    return 0;
                }

                if (Rhs == 3)
                {
                    int *piAddressVarThree = NULL;

                    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        return 0;
                    }

                    if (!isStringType(pvApiCtx, piAddressVarThree))
                    {
                        Scierror(
                                 999,
                                 _("%s: Wrong type for argument %d: A single string.\n"),
                                 fname, 3);
                        freeArrayOfWideString(pStVarOne, m1 * n1);
                        FREE(lenStVarOne);
                        return 0;
                    }

                    int ret = getAllocatedSingleString(pvApiCtx, piAddressVarThree, &functionName);
                    if (ret)
                    {
                        Scierror(
                                 999,
                                 _("%s: Wrong type for argument %d: A single string.\n"),
                                 fname, 3);
                        freeArrayOfWideString(pStVarOne, m1 * n1);
                        FREE(lenStVarOne);
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
                    Scierror(999, "%s: %s\n", fname, exception.what());
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
                Scierror(999, "%s: %s\n", fname, exception.what());
            }
        }

        freeArrayOfWideString(pStVarOne, m1 * n1);
        FREE(lenStVarOne);
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
