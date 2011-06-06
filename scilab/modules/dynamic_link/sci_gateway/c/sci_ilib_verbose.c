/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "gw_dynamic_link.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"
#include "ilib_verbose.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_ilib_verbose(char *fname, int* _piKey)
{
    SciErr sciErr;
    int iRet        = 0;
    int* piAddress  = NULL;
    double dblLevel = 0;
    int iLevel      = 0;

    CheckRhs(0,1);
    CheckLhs(1,1);

    if(Rhs == 0)
    {
        if(createScalarDouble(_piKey, Rhs + 1, (double)getIlibVerboseLevel()))
        {
            return 1;
        }

        LhsVar(1) = Rhs + 1;
    }
    else
    {
        sciErr = getVarAddressFromPosition(_piKey, 1, &piAddress);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }

        if(isDoubleType(_piKey, piAddress) == 0)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A int expected.\n"),fname,1);
            return 1;
        }

        if(getScalarDouble(_piKey, piAddress, &dblLevel))
        {
            return 1;
        }

        iLevel = (int)dblLevel;
        if(dblLevel != (double)iLevel)
        {
            Scierror(999,_("%s: Wrong value for input argument #%d: A int expected.\n"),fname,1);
            return 0;
        }

        if(!setIlibVerboseLevel((ilib_verbose_level)iLevel))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), fname, 1, "0,1,2");
            return 1;
        }
        else
        {
            LhsVar(1) = 0;
        }
    }

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
