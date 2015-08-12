/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_windows_tools.h"
#include "registry.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_winqueryreg(char *fname, unsigned long l)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;
    int *piAddressVarThree = NULL;

    char *pStrParamOne = NULL;
    char *pStrParamTwo = NULL;
    char *pStrParamThree = NULL;

    char *pStrOutput = NULL;
    int iOutput = 0;

    Rhs = Max(0, Rhs);
    CheckRhs(2, 3);
    CheckLhs(0, 1);

    if (Rhs == 3)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if (!isStringType(pvApiCtx, piAddressVarThree))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 3);
            return 0;
        }

        if (!isScalar(pvApiCtx, piAddressVarThree))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname, 3);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarThree, &pStrParamThree) != 0)
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
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

    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (!isStringType(pvApiCtx, piAddressVarTwo))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 2);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarTwo))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &pStrParamTwo) != 0)
    {
        if (pStrParamThree)
        {
            freeAllocatedSingleString(pStrParamThree);
            pStrParamThree = NULL;
        }
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &pStrParamOne) != 0)
    {
        if (pStrParamThree)
        {
            freeAllocatedSingleString(pStrParamThree);
            pStrParamThree = NULL;
        }

        if (pStrParamTwo)
        {
            freeAllocatedSingleString(pStrParamTwo);
            pStrParamTwo = NULL;
        }

        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    if (Rhs == 3)
    {
        BOOL bKey = strcmp(pStrParamOne, "key") == 0;
        BOOL bValue = strcmp(pStrParamOne, "name") == 0;

        if (bValue || bKey)
        {
            int NumbersElm = 0;
            if (bValue)
            {
                WindowsQueryRegistryNumberOfValuesInList(pStrParamTwo, pStrParamThree, &NumbersElm);
            }
            else
            {
                WindowsQueryRegistryNumberOfKeysInList(pStrParamTwo, pStrParamThree, &NumbersElm);
            }

            if (NumbersElm)
            {
                BOOL bResult = FALSE;
#define MAX_ELMT_REGLIST 255
                char **ListKeysName = NULL;
                int i = 0;

                if (NumbersElm > MAX_ELMT_REGLIST)
                {
                    NumbersElm = MAX_ELMT_REGLIST;
                }
                ListKeysName = (char **)MALLOC(sizeof(char*) * NumbersElm);
                for (i = 0; i < NumbersElm; i++)
                {
                    ListKeysName[i] = NULL;
                }

                if (bValue)
                {
                    bResult = WindowsQueryRegistryValuesList(pStrParamTwo, pStrParamThree, NumbersElm, ListKeysName);
                }
                else
                {
                    bResult = WindowsQueryRegistryKeysList(pStrParamTwo, pStrParamThree, NumbersElm, ListKeysName);
                }

                if (bResult)
                {
                    int nOne = 1;
                    sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, NumbersElm, nOne, ListKeysName);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    }
                    else
                    {
                        LhsVar(1) = Rhs + 1;
                        PutLhsVar();
                    }
                }
                else
                {
                    Scierror(999, _("%s: Cannot open Windows registry.\n"), fname);
                }
                freeArrayOfString(ListKeysName, NumbersElm);
            }
            else
            {
                createEmptyMatrix(pvApiCtx, Rhs + 1);
                LhsVar(1) = Rhs + 1;
                PutLhsVar();
            }

            if (pStrParamThree)
            {
                freeAllocatedSingleString(pStrParamThree);
                pStrParamThree = NULL;
            }

            if (pStrParamTwo)
            {
                freeAllocatedSingleString(pStrParamTwo);
                pStrParamTwo = NULL;
            }

            if (pStrParamOne)
            {
                freeAllocatedSingleString(pStrParamOne);
                pStrParamOne = NULL;
            }
            return 0;
        }
    }

    pStrOutput = (char*)MALLOC(PATH_MAX * sizeof(char));
    if (pStrOutput)
    {
        BOOL OuputIsREG_SZ = FALSE;
        BOOL TestWinQuery = WindowsQueryRegistry(pStrParamOne, pStrParamTwo, pStrParamThree, pStrOutput, &iOutput, &OuputIsREG_SZ);
        if ( TestWinQuery )
        {
            if (OuputIsREG_SZ)
            {
                createSingleString(pvApiCtx, Rhs + 1, pStrOutput);
            }
            else
            {
                createScalarDouble(pvApiCtx, Rhs + 1, (double)iOutput);
            }

            LhsVar(1) = Rhs + 1;
            PutLhsVar();
        }
        else
        {
            Scierror(999, _("%s: Cannot query value of this type.\n"), fname);
        }

        FREE( pStrOutput);
        pStrOutput = NULL;
    }
    else
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
    }

    if (pStrParamThree)
    {
        freeAllocatedSingleString(pStrParamThree);
        pStrParamThree = NULL;
    }

    if (pStrParamTwo)
    {
        freeAllocatedSingleString(pStrParamTwo);
        pStrParamTwo = NULL;
    }

    if (pStrParamOne)
    {
        freeAllocatedSingleString(pStrParamOne);
        pStrParamOne = NULL;
    }

    return 0;

}
/*--------------------------------------------------------------------------*/
