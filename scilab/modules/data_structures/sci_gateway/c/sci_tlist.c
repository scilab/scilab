/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "gw_data_structures1.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
extern int C2F(scilist)();
/*--------------------------------------------------------------------------*/
int sci_scitlist(char *fname, unsigned long fname_len)
{
    if (Rhs >= 1)
    {
#define RLIST_FIELDNAME "r"
        SciErr sciErr;
        int *piAddressVarOne = NULL;


        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarOne))
        {
            int m = 0;
            int n = 0;
            char **pStrs = NULL;
            BOOL bIsRfield = FALSE;

            if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarOne, &m, &n, &pStrs) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            bIsRfield = (BOOL)(strcmp(pStrs[0], RLIST_FIELDNAME) == 0);
            freeAllocatedMatrixOfString(m, n, pStrs);
            pStrs = NULL;

            // it is not allowed to create a tlist of type 'r'
            // 'r' type reserved to rlist
            if (bIsRfield)
            {
                Scierror(999, _("%s: Can not create a tlist with input argument #%d.\n"), fname, 1);
                return 0;
            }
        }
    }
    C2F(scilist)(fname, fname_len);
    return 0;
}
/*--------------------------------------------------------------------------*/
