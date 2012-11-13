/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
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
#include "dynamic_link_gw.hxx"
#include "function.hxx"
#include "double.hxx"
extern "C"
{
#include "ilib_verbose.h"
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_ilib_verbose(types::typed_list &in, int _iRetCount, types::typed_list &out)
{

    if(in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "ilib_verbose", 0, 1);
        return types::Function::Error;
    }

    if(in.size() == 0)
    {
        out.push_back(new types::Double((double)getIlibVerboseLevel()));
        return types::Function::OK;
    }

    if(in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: A int expected.\n"), "ilib_verbose", 1);
        return types::Function::Error;
    }

    int iLevel = (int) in[0]->getAs<types::Double>()->get(0);
    if((double)iLevel !=  in[0]->getAs<types::Double>()->get(0))
    {//not really a int
        Scierror(999, _("%s: Wrong value for input argument #%d: A int expected.\n"), "ilib_verbose", 1);
        return types::Function::Error;
    }

    if(!setIlibVerboseLevel((ilib_verbose_level)iLevel))
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "ilib_verbose", 1, "0,1,2");
        return types::Function::Error;
    }

    return types::Function::OK;
    //SciErr sciErr;
    //int iRet        = 0;
    //int* piAddress  = NULL;
    //double dblLevel = 0;
    //int iLevel      = 0;

    //CheckRhs(0,1);
    //CheckLhs(1,1);

    //if(Rhs == 0)
    //{
    //    if(createScalarDouble(pvApiCtx, Rhs + 1, (double)getIlibVerboseLevel()))
    //    {
    //        return 1;
    //    }

    //    LhsVar(1) = Rhs + 1;
    //}
    //else
    //{
    //    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddress);
    //    if(sciErr.iErr)
    //    {
    //        printError(&sciErr, 0);
    //        return sciErr.iErr;
    //    }

    //    if(isDoubleType(pvApiCtx, piAddress) == 0)
    //    {
    //        Scierror(999,_("%s: Wrong type for input argument #%d: A int expected.\n"),fname,1);
    //        return 1;
    //    }

    //    if(getScalarDouble(pvApiCtx, piAddress, &dblLevel))
    //    {
    //        return 1;
    //    }

    //    iLevel = (int)dblLevel;
    //    if(dblLevel != (double)iLevel)
    //    {
    //        Scierror(999,_("%s: Wrong value for input argument #%d: A int expected.\n"),fname,1);
    //        return 0;
    //    }

    //    if(!setIlibVerboseLevel((ilib_verbose_level)iLevel))
    //    {
    //        Scierror(999, _("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), fname, 1, "0,1,2");
    //        return 1;
    //    }
    //    else
    //    {
    //        LhsVar(1) = 0;
    //    }
    //}

    //PutLhsVar();
    //return 0;
}
/*--------------------------------------------------------------------------*/
