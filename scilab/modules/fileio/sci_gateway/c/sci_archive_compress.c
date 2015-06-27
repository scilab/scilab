/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Kartik Gupta
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "gw_fileio.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
#include "freeArrayOfString.h"
#include "expandPathVariable.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include <unistd.h>
#include "isdir.h"
#include "createdirectory.h"
#include "scicurdir.h"
#include "archive_compress.h"
/*--------------------------------------------------------------------------*/
int sci_archive_compress(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    char *pStVarOne = NULL;
    char *pStVarOneExpanded = NULL;

    int *piAddressVarTwo = NULL;
    char **pStVarTwo = NULL;
    int m1 = 0;
    int n1 = 0;
    int i = 0;

    int *piAddressVarThree = NULL;
    char *pStVarThree = NULL;

    char **file_list = NULL;
    /* Check Input & Output parameters */
    CheckRhs(3, 3);
    CheckLhs(1, 2);
    int result = 0;
    int *error = 0;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne) == 0 || isScalar(pvApiCtx, piAddressVarOne) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }


    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddressVarTwo))
    {
        if (isEmptyMatrix(pvApiCtx, piAddressVarTwo))
        {
            if (createEmptyMatrix(pvApiCtx, Rhs + 2))
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
            return 0;
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarTwo) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarTwo, &m1, &n1, &pStVarTwo))
    {
        freeAllocatedMatrixOfString(m1, n1, pStVarTwo);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }


    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarThree) == 0 || isScalar(pvApiCtx, piAddressVarThree) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 3);
        return 0;
    }


    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &pStVarOne))
    {
        if (pStVarOne)
        {
            freeAllocatedSingleWideString(pStVarOne);
        }

        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarThree, &pStVarThree))
    {
        if (pStVarThree)
        {
            freeAllocatedSingleWideString(pStVarThree);
        }

        freeAllocatedSingleString(pStVarOne);
        freeAllocatedSingleString(pStVarTwo);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }


    int size = 0;
    while (i < (m1 * n1))
    {
        size++;
        if (file_list != NULL)
        {
            file_list = (char**)REALLOC(file_list, sizeof(char*) * (size));
        }
        else
        {
            file_list = (char**)MALLOC(sizeof(char*) * (size));
        }
        file_list[size - 1] = (char*)MALLOC(sizeof(char) * 512);
        strcpy(file_list[size - 1], expandPathVariable(pStVarTwo[i]));
        i++;
    }


    i = 0;
    if (strlen(pStVarThree) > 2)
    {
        Scierror(999, _("%s: Sorry unrecognised format type.\n"), fname);
        return 0;
    }
    while (i < strlen(pStVarThree))
    {
        if (pStVarThree[i] != 'Z' && pStVarThree[i] != 'j' && pStVarThree[i] != 'y' && pStVarThree[i] != 'z')
        {
            Scierror(999, _("%s: Sorry unrecognised format type.\n"), fname);
            return 0;
        }
        i++;
    }
    result = archive_compress(pStVarOne, file_list, size, pStVarThree, &error);


    if (error == 1)
    {
        Scierror(999, _("%s: Sorry the file could not be opened.\n"), fname);
        return 0;
    }
    if (error == 2)
    {
        Scierror(999, _("%s: Sorry the file header could not be read\n"), fname);
        return 0;
    }
    if (error == 3)
    {
        Scierror(999, _("%s: Sorry the file header could not be written.\n"), fname);
        return 0;
    }

    freeAllocatedSingleString(pStVarOne);
    freeAllocatedSingleString(pStVarTwo);
    createScalarInteger32(pvApiCtx, Rhs + 1, result);
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;

}
/*--------------------------------------------------------------------------*/


