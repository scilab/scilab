/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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

    if (_iRetCount == 2)
    {
        out.push_back(new types::Double((double)getmemorysize()));
    }

    return types::Function::OK;
}
