/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - Allan CORNET - 2009
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
#include "stack-c.h"
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
    int iType1 = 0;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (iType1 == sci_strings)
    {
        wchar_t *pStVarOne = NULL;
        int lenStVarOne = 0;
        int m1 = 0, n1 = 0;
        BOOL *values = NULL;
        int valuesSize = 0;

        sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if ( m1 * n1 != 1 ) 
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
            return 0;
        }

        pStVarOne = (wchar_t*)MALLOC(sizeof(wchar_t) * (lenStVarOne + 1));
        if (pStVarOne == NULL)
        {
            Scierror(999,_("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
        if(sciErr.iErr)
        {
            FREE(pStVarOne);
            pStVarOne = NULL;
            printError(&sciErr, 0);
            return 0;
        }

        values = IsDigitW(pStVarOne, &valuesSize);

        if (pStVarOne) 
        {
            FREE(pStVarOne); 
            pStVarOne = NULL;
        }

        if (values)
        {
            m1 = 1;
            n1 = valuesSize;
            sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, m1, n1, values);

            if (values) 
            {
                FREE(values);
                values = NULL;
            }

            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }
        else
        {
            if (values) 
            {
                FREE(values);
                values = NULL;
            }
            m1 = 0;
            n1 = 0;

            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, NULL);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
        }

        LhsVar(1) = Rhs+1;
        C2F(putlhsvar)();
    }
    else
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/

