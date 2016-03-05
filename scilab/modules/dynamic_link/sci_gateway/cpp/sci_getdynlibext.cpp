/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
#include "string.hxx"
extern "C"
{
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_getdynlibext(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "getdynlibext", 0);
        return types::Function::Error;
    }

    out.push_back(new types::String(SHARED_LIB_EXTW));
    return types::Function::OK;
    //CheckRhs(0,0);
    //CheckLhs(1,1);

    //   createSingleString(pvApiCtx, Rhs + 1, SHARED_LIB_EXT);

    //LhsVar(1) = Rhs+1;
    //PutLhsVar();

    //return 0;
}
/*--------------------------------------------------------------------------*/
