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
#include "archive_list.h"
#include "freeArrayOfString.h"
#include "expandPathVariable.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int sci_archive_list(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    char **pStVarOne = NULL;
    int mOne = 0, nOne = 0;
    int mnOne = 0;

    char **fileList = NULL;
    int i = 0;
    char *pathextended;
    int rows, cols, error;

    Rhs = Max(Rhs, 0);
    CheckRhs(1, 1);
    CheckLhs(0, 1);
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        if (isEmptyMatrix(pvApiCtx, piAddressVarOne))
        {
            createEmptyMatrix(pvApiCtx, Rhs + 1);
            LhsVar(1) = Rhs + 1;
            PutLhsVar()
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        }
        return 0;
    }
    if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarOne, &mOne, &nOne, &pStVarOne) != 0)
    {
        freeAllocatedMatrixOfString(mOne, nOne, pStVarOne);
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }
    cols = 1;
    fileList = NULL;
    pathextended = expandPathVariable(pStVarOne[0]);
    fileList = (char**)(archive_list(pathextended,&rows,&error));
    freeAllocatedMatrixOfString(mOne, nOne, pStVarOne);
    if(error == 1)
    {
        Scierror(999, _("%s: Sorry the archive could not be opened\n"), fname);
        return 0;
    }
    else if(error == 2)
    {
	    Scierror(999, _("%s: Sorry the archive could not be closed.\n"), fname);
	    return 0;
    }
    CreateVarFromPtr(Rhs + 1, MATRIX_OF_STRING_DATATYPE, &rows, &cols, fileList);
    LhsVar(1) = Rhs + 1;

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/


