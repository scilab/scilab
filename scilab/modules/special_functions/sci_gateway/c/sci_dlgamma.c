/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 * Copyright (C) 2012 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "gw_special_functions.h"
#include "machine.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intsdlgamma)(char *id,unsigned long fname_len); /* fortran subroutine */
/*--------------------------------------------------------------------------*/
int sci_dlgamma(char *fname,unsigned long fname_len)
{
    if (Rhs == 1)
    {
        int *piAddressVarOne = NULL;
        int iType1 = 0;
        SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if ((iType1 == sci_list) ||
            (iType1 == sci_tlist) ||
            (iType1 == sci_mlist))
        {
            OverLoad(1);
            return 0;
        }
    }
    C2F(intsdlgamma)(fname, fname_len);
    return 0;
}
/*--------------------------------------------------------------------------*/
