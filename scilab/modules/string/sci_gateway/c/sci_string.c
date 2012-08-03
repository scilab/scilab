/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2011-2012 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_string.h"
#include "localization.h"
#include "Scierror.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intstring) (void); /* fortran routine */
/*--------------------------------------------------------------------------*/
int sci_string(char *fname, unsigned long fname_len)
{
    CheckRhs(1, 1);
    CheckLhs(1, 3);

    switch(VarType(1))
    {
    case sci_matrix:
        {
            if (Lhs == 1)
            {
                SciErr sciErr;
                int iRows = 0, iCols = 0;
                int *piAddressVarOne = NULL;

                sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    return 0;
                }

                sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &iRows, &iCols);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    return 0;
                }

                /* case string(A) returns '' */
                /* with A has special dimensions (-1,-1) */
                /* bug 3747, 10641 */
                if ( (iRows == -1) && (iCols == -1) ) 
                {
                    if (createSingleString(pvApiCtx, Rhs + 1, "") != 0)
                    {
                        Scierror(999, _("%s: No more memory.\n"), fname);
                        return 0;
                    }
                    LhsVar(1) = Rhs + 1;
                    PutLhsVar();
                }
                else
                {
                    C2F(intstring)();
                }
            }
            else SciError(39);
        }
        break;

    case sci_poly: 
    case sci_boolean:
    case sci_sparse: 
    case sci_boolean_sparse:
    case sci_matlab_sparse: 
    case sci_ints:
    case sci_handles: 
    case sci_strings:
    case sci_list: 
    case sci_tlist:
    case sci_mlist: 
    case sci_lufact_pointer:
        {
            if (Lhs == 1)
            {
                C2F(intstring)();
            }
            else
            {
                SciError(39);
            }
        }
        break;

    case sci_u_function: 
        {
            if (Lhs == 3)
            {
                C2F(intstring)();
            }
            else
            {
                SciError(41);
            }
        }
        break;
    case sci_c_function:
        {
            /* macros */
            if (Lhs == 3)
            {
                OverLoad(1);
            }
            else 
            {
                SciError(41);
            }
        }
        break;
    case sci_lib:
        {
            if (Lhs == 1)
            {
                C2F(intstring)();
            }
            else SciError(39);
        }
        break;

    case sci_implicit_poly:
        {
            if (Lhs == 1)
            {
                OverLoad(1);
            }
            else 
            {
                SciError(41);
            }
        }
        break;
    default:
        {
            Scierror(999,_("%s: Wrong type for input argument #%d.\n"), fname, 1);
        }
        break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
