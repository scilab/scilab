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
#include <string.h>
#include "gw_fileio.h"
#include "Scierror.h"
#include "scicurdir.h"
#include "localization.h"
#include "expandPathVariable.h"
#include "sci_malloc.h"
#include "localization.h"
#include "api_scilab.h"
#include "isdir.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
int sci_chdir(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    char* pStVarOne = NULL;
    int iType1	= 0;
    int lenStVarOne = 0;
    int m1 = 0, n1 = 0;

    char* expandedPath = NULL;

    CheckRhs(0, 1);
    CheckLhs(1, 1);

    if (Rhs == 0)
    {
        pStVarOne = (char*)MALLOC(sizeof(char) * ((int)strlen("home") + 1));
        if (pStVarOne)
        {
            strcpy(pStVarOne, "home");
        }
    }
    else
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarOne) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &pStVarOne))
        {
            if (pStVarOne)
            {
                freeAllocatedSingleString(pStVarOne);
            }

            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }
    }

    expandedPath = expandPathVariable(pStVarOne);
    freeAllocatedSingleString(pStVarOne);

    if (expandedPath == NULL)
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    /* get value of PWD scilab variable (compatiblity scilab 4.x) */
    if (strcmp(expandedPath, "PWD") == 0)
    {
        sciErr = getNamedVarType(pvApiCtx, "PWD", &iType1);
        if (sciErr.iErr)
        {
            FREE(expandedPath);
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read named argument %s.\n"), fname, "PWD");
            return 0;
        }

        if (iType1 == sci_strings)
        {
            char* VARVALUE = NULL;
            FREE(expandedPath);
            if (getAllocatedNamedSingleString(pvApiCtx, "PWD", &VARVALUE))
            {
                if (VARVALUE)
                {
                    freeAllocatedSingleString(VARVALUE);
                }

                Scierror(999, _("%s: Can not read named argument %s.\n"), fname, "PWD");
                return 0;
            }

            expandedPath = VARVALUE;
        }
    }

    if (strcmp(fname, "chdir") == 0) /* chdir output boolean */
    {
        int iOutput = FALSE;
        if (scichdir(expandedPath))
        {
            iOutput = FALSE;
        }
        else
        {
            iOutput = TRUE;
        }

        if (createScalarBoolean(pvApiCtx, Rhs + 1, iOutput))
        {
            FREE(expandedPath);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        FREE(expandedPath);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    else /* cd output string current path */
    {
        if ( isdir(expandedPath) || (strcmp(expandedPath, "/") == 0) ||
                (strcmp(expandedPath, "\\") == 0) )
        {
            int ierr = scichdir(expandedPath);
            char* currentDir = scigetcwd(&ierr);
            if ( (ierr == 0) && currentDir)
            {
                sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, 1, 1, (char const * const*) &currentDir);
            }
            else
            {
                sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, NULL);
            }

            FREE(expandedPath);
            FREE(currentDir);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(1) = Rhs + 1;
            PutLhsVar();
        }
        else
        {
            Scierror(998, _("%s: Cannot go to directory %s\n"), fname, expandedPath);
            FREE(expandedPath);
            expandedPath = NULL;
        }
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
