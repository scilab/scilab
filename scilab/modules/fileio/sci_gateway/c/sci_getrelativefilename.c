/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Pierre MARECHAL
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string.h>
#include "gw_io.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "getrelativefilename.h"
#include "PATH_MAX.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
/* ================================================================================== */
// sci_getrelativefilename
//
// scilab interface to getrelativefilename
/* ================================================================================== */
int sci_getrelativefilename(char *fname, unsigned long l)
{

    SciErr sciErr;
    int* piAddr1 = NULL;
    int* piAddr2 = NULL;
    int iRhs = nbInputArgument(pvApiCtx);
    char **Input_StringMatrix_1 = NULL; //Input value
    char **Input_StringMatrix_2 = NULL; //Input value
    int iRows1 = 0, iRows2 = 0;
    int iCols1 = 0, iCols2 = 0;
    int x; //loop index
    char **OutputStrings = NULL; //Output value

    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1);

    // get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }
    // Check type
    if (isStringType(pvApiCtx, piAddr1) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of strings expected.\n"), fname, 1);
        return 0;
    }
    if (getAllocatedMatrixOfString(pvApiCtx, piAddr1, &iRows1, &iCols1, &Input_StringMatrix_1))
    {
        freeAllocatedMatrixOfString(iRows1, iCols1, Input_StringMatrix_1);
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of strings expected.\n"), fname, 1);
        return 0;
    }

    // get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        freeAllocatedMatrixOfString(iRows1, iCols1, Input_StringMatrix_1);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }
    // Check type
    if (isStringType(pvApiCtx, piAddr2) == 0)
    {
        freeAllocatedMatrixOfString(iRows1, iCols1, Input_StringMatrix_1);
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of strings expected.\n"), fname, 2);
        return 0;
    }
    if (getAllocatedMatrixOfString(pvApiCtx, piAddr2, &iRows2, &iCols2, &Input_StringMatrix_2))
    {
        freeAllocatedMatrixOfString(iRows1, iCols1, Input_StringMatrix_1);
        freeAllocatedMatrixOfString(iRows2, iCols2, Input_StringMatrix_2);
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of strings expected.\n"), fname, 2);
        return 0;
    }

    if (iRows1 != iRows2 || iCols1 != iCols2)
    {
        freeAllocatedMatrixOfString(iRows1, iCols1, Input_StringMatrix_1);
        freeAllocatedMatrixOfString(iRows2, iCols2, Input_StringMatrix_2);
        Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same size expected.\n"), fname, 1, 2);
        return 0;
    }

    OutputStrings = (char**)MALLOC(iRows1 * iCols1 * sizeof(char*));
    if (OutputStrings == NULL)
    {
        freeAllocatedMatrixOfString(iRows1, iCols1, Input_StringMatrix_1);
        freeAllocatedMatrixOfString(iRows2, iCols2, Input_StringMatrix_2);
        Scierror(112, _("%s: No more memory.\n"), fname);
        return 0;
    }

    for ( x = 0; x < iRows1 * iCols1; x++)
    {
        // make sure the names are not too long
        if ( strlen(Input_StringMatrix_1[x]) > PATH_MAX )
        {
            freeAllocatedMatrixOfString(iRows1, iCols1, Input_StringMatrix_1);
            freeAllocatedMatrixOfString(iRows2, iCols2, Input_StringMatrix_2);
            freeArrayOfString(OutputStrings, iRows1 * iCols1);
            Scierror(999, _("%s: Wrong size for input argument #%d: Must be less than %d characters.\n"), fname, 1, PATH_MAX);
            return 0;
        }

        if ( strlen(Input_StringMatrix_2[x]) > PATH_MAX )
        {
            freeAllocatedMatrixOfString(iRows1, iCols1, Input_StringMatrix_1);
            freeAllocatedMatrixOfString(iRows2, iCols2, Input_StringMatrix_2);
            freeArrayOfString(OutputStrings, iRows1 * iCols1);
            Scierror(999, _("%s: Wrong size for input argument #%d: Must be less than %d characters.\n"), fname, 2, PATH_MAX);
            return 0;
        }

        OutputStrings[x] = getrelativefilename(Input_StringMatrix_1[x], Input_StringMatrix_2[x]);
    }

    freeAllocatedMatrixOfString(iRows1, iCols1, Input_StringMatrix_1);
    freeAllocatedMatrixOfString(iRows2, iCols2, Input_StringMatrix_2);

    sciErr = createMatrixOfString(pvApiCtx, iRhs + 1, iRows1, iCols1, (char**)OutputStrings);
    if (sciErr.iErr)
    {
        freeArrayOfString(OutputStrings, iRows1 * iCols1);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    freeArrayOfString(OutputStrings, iRows1 * iCols1);

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    ReturnArguments(pvApiCtx);
    return 0;
}
