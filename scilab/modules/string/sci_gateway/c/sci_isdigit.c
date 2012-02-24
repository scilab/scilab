/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - Allan CORNET - 2009-2011
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*-----------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "BOOL.h"
#include "isdigit.h"
#include "MALLOC.h"
/*----------------------------------------------------------------------------*/
int sci_isdigit(char *fname,unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isScalar(pvApiCtx, piAddressVarOne))
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
    }
    else
    {
        wchar_t *pStVarOne = NULL;

        if (getAllocatedSingleWideString(pvApiCtx, piAddressVarOne, &pStVarOne) != 0)
        {
            Scierror(999,_("%s: Memory allocation error.\n"), fname);
            return 0;
        }
        else
        {
            int valuesSize = 0;
            BOOL *values = IsDigitW(pStVarOne, &valuesSize);

            freeAllocatedSingleWideString(pStVarOne);
            pStVarOne = NULL;

            if (values)
            {
                int m1 = 1;
                int n1 = valuesSize;
                sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, m1, n1, values);

                FREE(values);
                values = NULL;

                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999,_("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
            }
            else
            {
                createEmptyMatrix(pvApiCtx, Rhs + 1);
            }

            LhsVar(1) = Rhs+1;
            PutLhsVar();
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/

