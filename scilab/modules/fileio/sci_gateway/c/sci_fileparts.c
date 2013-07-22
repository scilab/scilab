/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009-2011 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

static int allocStrings(void** _pwstDrv, void** _pwstDir, void** _pwstName, void** _pwstExt, void** _pwstPath, int _iNum, int _iSize);
static void freeStrings(void** _pwstDrv, void** _pwstDir, void** _pwstName, void** _pwstExt, void** _pwstPath);

/*--------------------------------------------------------------------------*/
int sci_fileparts(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int *piAddressVarOne = NULL;
    wchar_t **pStVarOne = NULL;
    wchar_t *pStVarTwo = NULL;
    int *piAddressVarTwo = NULL;

    wchar_t **drv = NULL;
    wchar_t **dir = NULL;
    wchar_t **name = NULL;
    wchar_t **ext = NULL;
    wchar_t **path_out = NULL;
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

        if (getAllocatedSingleWideString(pvApiCtx, piAddressVarTwo, &pStVarTwo))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }
    }

    if (allocStrings((void**)&drv, (void**)&dir, (void**)&name, (void**)&ext, (void**)&path_out, iRows * iCols, sizeof(wchar_t**)))
    {
        if (pStVarOne)
        {
            freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
            pStVarOne = NULL;
        }

        if (pStVarTwo)
        {
            freeAllocatedSingleWideString(pStVarTwo);
            pStVarTwo = NULL;
        }

        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    if (pStVarTwo)              /* Rhs == 2 */
    {
        output_value = (wchar_t **)MALLOC(sizeof(wchar_t*) * iRows * iCols);
        if (output_value == NULL)
        {
            freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
            freeAllocatedSingleWideString(pStVarTwo);
            freeStrings((void**)&drv, (void**)&dir, (void**)&name, (void**)&ext, (void**)&path_out);
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        for (i = 0; i < iRows * iCols; i++)
        {
            int iLen        = (int)wcslen(pStVarOne[i]) + 1;
            output_value[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * iLen);

            if (allocStrings((void**)&drv[i], (void**)&dir[i], (void**)&name[i], (void**)&ext[i], (void**)&path_out[i], iLen, sizeof(wchar_t*))
                    || output_value[i] == NULL)
            {
                if (pStVarOne[i])
                {
                    freeArrayOfWideString(pStVarOne, i);
                    pStVarOne[i] = NULL;
                }
                if (pStVarTwo)
                {
                    freeAllocatedSingleWideString(pStVarTwo);
                    pStVarTwo = NULL;
                }

                if (output_value[i])
                {
                    freeArrayOfWideString(output_value, i);
                    output_value[i] = NULL;
                }

                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            splitpathW(pStVarOne[i], FALSE, drv[i], dir[i], name[i], ext[i]);

            if (wcscmp(pStVarTwo, FILEPARTS_PATH_SELECTOR) == 0)
            {
                output_value = path_out;
                wcscpy(output_value[i], drv[i]);
                wcscat(output_value[i], dir[i]);
            }
            else if (wcscmp(pStVarTwo, FILEPARTS_FNAME_SELECTOR) == 0)
            {
                output_value[i] = name[i];
            }
            else if (wcscmp(pStVarTwo, FILEPARTS_EXTENSION_SELECTOR) == 0)
            {
                output_value[i] = ext[i];
            }
            else
            {
                int j = 0;
                if (pStVarTwo[i])
                {
                    freeAllocatedSingleWideString(pStVarTwo);
                    pStVarTwo = NULL;
                }

                for (j = 0 ; j <= i ; j++)
                {
                    freeStrings((void**)&drv[j], (void**)&dir[j], (void**)&name[j], (void**)&ext[j], (void**)&path_out[j]);
                }

                freeStrings((void**)&drv, (void**)&dir, (void**)&name, (void**)&ext, (void**)&path_out);

                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
                return 0;
            }

        }
        sciErr = createMatrixOfWideString(pvApiCtx, iRhs + 1, iRows, iCols, output_value);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
        AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;

        if (pStVarTwo)
        {
            freeAllocatedSingleWideString(pStVarTwo);
            pStVarTwo = NULL;
        }
    }
    else
    {
        for (i = 0; i < iRows * iCols; i++)
        {

            int iLen    = (int)wcslen(pStVarOne[i]) + 1;
            if (allocStrings((void**)&drv[i], (void**)&dir[i], (void**)&name[i], (void**)&ext[i], (void**)&path_out[i], iLen, sizeof(wchar_t*)))
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            splitpathW(pStVarOne[i], FALSE, drv[i], dir[i], name[i], ext[i]);
            wcscpy(path_out[i], drv[i]);
            wcscat(path_out[i], dir[i]);
        }

        sciErr = createMatrixOfWideString(pvApiCtx, iRhs + 1, iRows, iCols, path_out);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;

        sciErr = createMatrixOfWideString(pvApiCtx, iRhs + 2, iRows, iCols, name);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        AssignOutputVariable(pvApiCtx, 2) = iRhs + 2;

        sciErr = createMatrixOfWideString(pvApiCtx, iRhs + 3, iRows, iCols, ext);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }


        AssignOutputVariable(pvApiCtx, 3) = iRhs + 3;
    }

    if (pStVarOne)
    {
        freeAllocatedMatrixOfWideString(iRows, iCols, pStVarOne);
        pStVarOne = NULL;
    }
    ReturnArguments(pvApiCtx);

    for (i = 0 ; i < iRows * iCols ; i++)
    {
        freeStrings((void**)&drv[i], (void**)&dir[i], (void**)&name[i], (void**)&ext[i], (void**)&path_out[i]);
    }

    freeStrings((void**)&drv, (void**)&dir, (void**)&name, (void**)&ext, (void**)&path_out);

    return 0;
}

static void* allocVariable(int _iSize, int _iNum)
{
    return MALLOC(_iSize * _iNum);
}

static void freeVariable(void** _pwstVar)
{
    if (*_pwstVar)
    {
        FREE(*_pwstVar);
        *_pwstVar = NULL;
    }
}

static void freeStrings(void** _pwstDrv, void** _pwstDir, void** _pwstName, void** _pwstExt, void** _pwstPath)
{
    freeVariable(_pwstDrv);
    freeVariable(_pwstDir);
    freeVariable(_pwstName);
    freeVariable(_pwstExt);
    freeVariable(_pwstPath);
}

static int allocStrings(void** _pwstDrv, void** _pwstDir, void** _pwstName, void** _pwstExt, void** _pwstPath, int _iNum, int _iSize)
{
    *_pwstDrv   = allocVariable(_iSize, _iNum);
    *_pwstDir   = allocVariable(_iSize, _iNum);
    *_pwstName  = allocVariable(_iSize, _iNum);
    *_pwstExt   = allocVariable(_iSize, _iNum);
    *_pwstPath  = allocVariable(_iSize, _iNum);

    if (*_pwstDrv == NULL || *_pwstDir == NULL || *_pwstName == NULL || *_pwstExt == NULL || *_pwstPath == NULL)
    {
        freeStrings((void**)_pwstDrv, (void**)_pwstDir, (void**)_pwstName, (void**)_pwstExt, (void**)_pwstPath);
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
