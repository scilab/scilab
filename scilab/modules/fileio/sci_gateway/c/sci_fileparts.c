/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009-2011 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "MALLOC.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "splitpath.h"
/*--------------------------------------------------------------------------*/
#define FILEPARTS_PATH_SELECTOR L"path"
#define FILEPARTS_FNAME_SELECTOR L"fname"
#define FILEPARTS_EXTENSION_SELECTOR L"extension"

static void allocStrings(wchar_t** _pwstDrv, wchar_t** _pwstDir, wchar_t** _pwstName, wchar_t** _pwstExt, wchar_t** _pwstPath, int _iNum);
static void freeStrings(wchar_t** _pwstDrv, wchar_t** _pwstDir, wchar_t** _pwstName, wchar_t** _pwstExt, wchar_t** _pwstPath);

/*--------------------------------------------------------------------------*/
int sci_fileparts(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int *piAddressVarOne = NULL;
    wchar_t **pStVarOne = NULL;
    int *piAddressVarTwo = NULL;
    wchar_t *pStVarTwo = NULL;

    wchar_t **output_value = NULL;
    int iRows = 0;
    int iCols = 0;
    int iRhs = nbInputArgument(pvApiCtx);
    int iLhs = nbOutputArgument(pvApiCtx);
    int i = 0; // loop index

    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 3);

    if ((iRhs == 2) && (iLhs != 1))
    {
        Scierror(78, _("%s: Wrong number of output arguments: %d expected.\n"), fname, 1);
        return 0;
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
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedMatrixOfWideString(pvApiCtx, piAddressVarOne, &iRows, &iCols, &pStVarOne))
    {
        freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"), fname, 1);
        return 0;
    }

    if (iRhs == 2)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (!isStringType(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        if (getAllocatedSingleWideString(pvApiCtx, piAddressVarTwo, &pStVarTwo))
        {
            freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
            freeAllocatedSingleWideString(pStVarTwo);
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }
    }

    if (pStVarTwo)              /* Rhs == 2 */
    {
        output_value = (wchar_t **)MALLOC(sizeof(wchar_t*) * iRows * iCols);
        if (output_value == NULL)
        {
            freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
            freeAllocatedSingleWideString(pStVarTwo);
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        for (i = 0; i < iRows * iCols; i++)
        {
            wchar_t *drv = NULL;
            wchar_t *dir = NULL;
            wchar_t *name = NULL;
            wchar_t *ext = NULL;

            int iLen        = (int)wcslen(pStVarOne[i]) + 1;
            output_value[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * iLen);

            if (output_value[i] == NULL)
            {
                freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
                freeAllocatedSingleWideString(pStVarTwo);
                freeArrayOfWideString(output_value, i);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            allocStrings(&drv, &dir, &name, &ext, NULL, iLen);
            splitpathW(pStVarOne[i], FALSE, drv, dir, name, ext);

            if (wcscmp(pStVarTwo, FILEPARTS_PATH_SELECTOR) == 0)
            {
                wcscpy(output_value[i], drv);
                wcscat(output_value[i], dir);
            }
            else if (wcscmp(pStVarTwo, FILEPARTS_FNAME_SELECTOR) == 0)
            {
                output_value[i] = name;
                //to avoid free
                name = NULL;
            }
            else if (wcscmp(pStVarTwo, FILEPARTS_EXTENSION_SELECTOR) == 0)
            {
                output_value[i] = ext;
                //to avoid free
                ext = NULL;
            }
            else
            {
                int j = 0;
                freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
                freeAllocatedSingleWideString(pStVarTwo);
                freeArrayOfWideString(output_value, i);
                freeStrings(&drv, &dir, &name, &ext, NULL);
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
                return 0;
            }

            freeStrings(&drv, &dir, &name, &ext, NULL);
        }

        sciErr = createMatrixOfWideString(pvApiCtx, iRhs + 1, iRows, iCols, output_value);
        freeArrayOfWideString(output_value, iRows * iCols);
        freeAllocatedSingleWideString(pStVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    }
    else
    {
        wchar_t **path_out = (wchar_t**)MALLOC(sizeof(wchar_t*) * iRows * iCols);
        wchar_t **name = (wchar_t**)MALLOC(sizeof(wchar_t*) * iRows * iCols);
        wchar_t **ext = (wchar_t**)MALLOC(sizeof(wchar_t*) * iRows * iCols);
        for (i = 0; i < iRows * iCols; i++)
        {
            wchar_t *drv = NULL;
            wchar_t *dir = NULL;

            int iLen = (int)wcslen(pStVarOne[i]) + 1;
            allocStrings(&drv, &dir, &name[i], &ext[i], &path_out[i], iLen);
            splitpathW(pStVarOne[i], FALSE, drv, dir, name[i], ext[i]);
            wcscpy(path_out[i], drv);
            wcscat(path_out[i], dir);

            freeStrings(&drv, &dir, NULL, NULL, NULL);
        }

        sciErr = createMatrixOfWideString(pvApiCtx, iRhs + 1, iRows, iCols, path_out);
        if (sciErr.iErr)
        {
            freeArrayOfWideString(path_out, i);
            freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        freeArrayOfWideString(path_out, iRows * iCols);
        AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;

        sciErr = createMatrixOfWideString(pvApiCtx, iRhs + 2, iRows, iCols, name);
        if (sciErr.iErr)
        {
            freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        freeArrayOfWideString(name, iRows * iCols);
        AssignOutputVariable(pvApiCtx, 2) = iRhs + 2;

        sciErr = createMatrixOfWideString(pvApiCtx, iRhs + 3, iRows, iCols, ext);
        if (sciErr.iErr)
        {
            freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        freeArrayOfWideString(ext, iRows * iCols);
        AssignOutputVariable(pvApiCtx, 3) = iRhs + 3;
    }

    ReturnArguments(pvApiCtx);
    return 0;
}

static void freeVariable(wchar_t** _pwstVar)
{
    if (_pwstVar && *_pwstVar)
    {
        FREE(*_pwstVar);
        *_pwstVar = NULL;
    }
}

static void freeStrings(wchar_t** _pwstDrv, wchar_t** _pwstDir, wchar_t** _pwstName, wchar_t** _pwstExt, wchar_t** _pwstPath)
{
    freeVariable(_pwstDrv);
    freeVariable(_pwstDir);
    freeVariable(_pwstName);
    freeVariable(_pwstExt);
    freeVariable(_pwstPath);
}

static void allocString(wchar_t** _pwst, int _iNum)
{
    if (_pwst)
    {
        *_pwst = (wchar_t*)MALLOC(sizeof(wchar_t) * _iNum);
    }
}

static void allocStrings(wchar_t** _pwstDrv, wchar_t** _pwstDir, wchar_t** _pwstName, wchar_t** _pwstExt, wchar_t** _pwstPath, int _iNum)
{
    allocString(_pwstDrv, _iNum);
    allocString(_pwstDir, _iNum);
    allocString(_pwstName, _iNum);
    allocString(_pwstExt, _iNum);
    allocString(_pwstPath, _iNum);
}
/*--------------------------------------------------------------------------*/
