/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2011 - Antoine ELIAS
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
#include "dynamic_link.h"
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_c_link(char *fname, int* _piKey)
{
    SciErr sciErr;
    int iRet = 0;
    int ilib = 0;
    int* piAddress;
    int iType;

    char *routinename   = NULL;
    int paramoutINT     = 0;
    BOOL FindFunction   = FALSE;

    CheckRhs(1,2);
    CheckLhs(1,2);

    /* [test,ilib]=c_link(routine-name)  */
    /* test = c_link(routine-name)  */
    /* test = c_link(routine-name,num)  */

    if(Rhs == 2)
    {
        sciErr = getVarAddressFromPosition(_piKey, 2, &piAddress);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
        sciErr = getVarType(_piKey, piAddress, &iType);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }

        if(isDoubleType(_piKey, piAddress))
        {
            double pdblReal = 0;
            if(getScalarDouble(_piKey, piAddress, &pdblReal))
            {
                Scierror(999, _("%s : second argument must be a unique id of a shared library.\n"),fname);
                return 1;
            }

            ilib = (int)pdblReal;
        }
        else
        {
            Scierror(999 ,_("%s : second argument must be a unique id of a shared library.\n"),fname);
            return 0;
        }
    }
    else 
    {
        ilib = -1;
    }

    sciErr = getVarAddressFromPosition(_piKey, 1, &piAddress);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if(isStringType(_piKey, piAddress) == FALSE)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 1;
    }

    if(getAllocatedSingleString(_piKey, piAddress, &routinename))
    {
        return 1;
    }

    FindFunction = c_link(routinename, &ilib);
    FREE(routinename);

    if(FindFunction)
    {
        paramoutINT = (int)(TRUE);
    }

    if(createScalarBoolean(_piKey, Rhs + 1, paramoutINT))
    {
        return 1;
    }

    LhsVar(1) = Rhs + 1;

    if(Lhs == 2)
    {
        if(createScalarDouble(_piKey, Rhs + 2, ilib))
        {
            return 1;
        }

        LhsVar(2) = Rhs + 2;
    }

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
