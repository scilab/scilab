/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "function.hxx"
#include "configvariable.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "getmemory.h"
}

types::Function::ReturnValue sci_getmemory(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected."), "funcprot", 0);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(77, _("%s: Wrong number of output argument(s): %d expected."), "funcprot", 2);
        return types::Function::Error;
    }

    out.push_back(new types::Double((double)getfreememory()));

    if(_iRetCount == 2)
    {
        out.push_back(new types::Double((double)getmemorysize()));
    }

    return types::Function::OK;
}
