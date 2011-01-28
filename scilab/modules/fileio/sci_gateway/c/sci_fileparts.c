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
#include "stack-c.h"
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
/*--------------------------------------------------------------------------*/
int sci_fileparts(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int *piAddressVarOne = NULL;
    wchar_t *pStVarOne = NULL;
    int lenStVarOne = 0;
    wchar_t *pStVarTwo = NULL;

    wchar_t *drv = NULL;
    wchar_t *dir = NULL;
    wchar_t *name = NULL;
    wchar_t *ext = NULL;
    wchar_t *path_out = NULL;

    CheckLhs(1, 3);
    CheckRhs(1, 2);

    if ((Rhs == 2) && (Lhs != 1))
    {
        Scierror(78, _("%s: Wrong number of output arguments: %d expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarOne))
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleWideString(pvApiCtx, piAddressVarOne, &pStVarOne))
        return 0;

    if (pStVarOne == NULL)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    lenStVarOne = (int)wcslen(pStVarOne);

    if (Rhs == 2)
    {
        int m2 = 0, n2 = 0;
        int *piAddressVarTwo = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (!isStringType(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        if (!isScalar(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }

        if (getAllocatedSingleWideString(pvApiCtx, piAddressVarTwo, &pStVarTwo))
        {
            if (pStVarOne)
            {
                freeAllocatedSingleWideString(pStVarOne);
                pStVarOne = NULL;
            }
            return 0;
        }
    }

    drv = (wchar_t *) MALLOC(sizeof(wchar_t) * (lenStVarOne + 1));
    dir = (wchar_t *) MALLOC(sizeof(wchar_t) * (lenStVarOne + 1));
    name = (wchar_t *) MALLOC(sizeof(wchar_t) * (lenStVarOne + 1));
    ext = (wchar_t *) MALLOC(sizeof(wchar_t) * (lenStVarOne + 1));
    path_out = (wchar_t *) MALLOC(sizeof(wchar_t) * (lenStVarOne + 1));

    if ((drv == NULL) || (dir == NULL) || (name == NULL) || (ext == NULL) || (path_out == NULL))
    {
        if (pStVarOne)
        {
            freeAllocatedSingleWideString(pStVarOne);
            pStVarOne = NULL;
        }
        if (pStVarTwo)
        {
            freeAllocatedSingleWideString(pStVarTwo);
            pStVarTwo = NULL;
        }
        if (drv)
        {
            FREE(drv);
            drv = NULL;
        }
        if (dir)
        {
            FREE(dir);
            dir = NULL;
        }
        if (name)
        {
            FREE(name);
            name = NULL;
        }
        if (ext)
        {
            FREE(ext);
            ext = NULL;
        }
        if (path_out)
        {
            FREE(path_out);
            path_out = NULL;
        }

        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    splitpathW(pStVarOne, FALSE, drv, dir, name, ext);
    if (pStVarOne)
    {
        freeAllocatedSingleWideString(pStVarOne);
        pStVarOne = NULL;
    }

    if (pStVarTwo)              /* Rhs == 2 */
    {
        wchar_t *output_value = NULL;

        if (wcscmp(pStVarTwo, FILEPARTS_PATH_SELECTOR) == 0)
        {
            output_value = path_out;
            wcscpy(output_value, drv);
            wcscat(output_value, dir);
        }
        else if (wcscmp(pStVarTwo, FILEPARTS_FNAME_SELECTOR) == 0)
        {
            output_value = name;
        }
        else if (wcscmp(pStVarTwo, FILEPARTS_EXTENSION_SELECTOR) == 0)
        {
            output_value = ext;
        }
        else
        {
            if (pStVarTwo)
            {
                freeAllocatedSingleWideString(pStVarTwo);
                pStVarTwo = NULL;
            }
            if (drv)
            {
                FREE(drv);
                drv = NULL;
            }
            if (dir)
            {
                FREE(dir);
                dir = NULL;
            }
            if (name)
            {
                FREE(name);
                name = NULL;
            }
            if (ext)
            {
                FREE(ext);
                ext = NULL;
            }
            if (path_out)
            {
                FREE(path_out);
                path_out = NULL;
            }

            Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (pStVarTwo)
        {
            freeAllocatedSingleWideString(pStVarTwo);
            pStVarTwo = NULL;
        }

        if (createSingleWideString(pvApiCtx, Rhs + 1, output_value) == 0)
        {
            LhsVar(1) = Rhs + 1;
            C2F(putlhsvar) ();
        }

        if (drv)
        {
            FREE(drv);
            drv = NULL;
        }
        if (dir)
        {
            FREE(dir);
            dir = NULL;
        }
        if (name)
        {
            FREE(name);
            name = NULL;
        }
        if (ext)
        {
            FREE(ext);
            ext = NULL;
        }
        if (path_out)
        {
            FREE(path_out);
            path_out = NULL;
        }
    }
    else
    {
        if (pStVarTwo)
        {
            freeAllocatedSingleWideString(pStVarTwo);
            pStVarTwo = NULL;
        }
        wcscpy(path_out, drv);
        wcscat(path_out, dir);

        if (drv)
        {
            FREE(drv);
            drv = NULL;
        }
        if (dir)
        {
            FREE(dir);
            dir = NULL;
        }

        if (createSingleWideString(pvApiCtx, Rhs + 1, path_out))
        {
            if (name)
            {
                FREE(name);
                name = NULL;
            }
            if (ext)
            {
                FREE(ext);
                ext = NULL;
            }
            if (path_out)
            {
                FREE(path_out);
                path_out = NULL;
            }
            return 0;
        }
        if (path_out)
        {
            FREE(path_out);
            path_out = NULL;
        }
        LhsVar(1) = Rhs + 1;

        if (createSingleWideString(pvApiCtx, Rhs + 2, name))
        {
            if (name)
            {
                FREE(name);
                name = NULL;
            }
            if (ext)
            {
                FREE(ext);
                ext = NULL;
            }
            return 0;
        }
        if (name)
        {
            FREE(name);
            name = NULL;
        }
        LhsVar(2) = Rhs + 2;

        if (createSingleWideString(pvApiCtx, Rhs + 3, ext))
        {
            if (ext)
            {
                FREE(ext);
                ext = NULL;
            }
            return 0;
        }
        if (ext)
        {
            FREE(ext);
            ext = NULL;
        }
        LhsVar(3) = Rhs + 3;
        C2F(putlhsvar) ();
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
