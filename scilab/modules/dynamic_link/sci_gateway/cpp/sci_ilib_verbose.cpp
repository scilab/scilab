/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 * Copyright (C) DIGITEO - 2011 - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "ilib_verbose", 0, 1);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        out.push_back(new types::Double((double)getIlibVerboseLevel()));
        return types::Function::OK;
    }

    if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: A int expected.\n"), "ilib_verbose", 1);
        return types::Function::Error;
    }

    int iLevel = (int) in[0]->getAs<types::Double>()->get(0);
    if ((double)iLevel !=  in[0]->getAs<types::Double>()->get(0))
    {
        //not really a int
        Scierror(999, _("%s: Wrong value for input argument #%d: A int expected.\n"), "ilib_verbose", 1);
        return types::Function::Error;
    }

    if (!setIlibVerboseLevel((ilib_verbose_level)iLevel))
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
