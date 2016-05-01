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
}

types::Function::ReturnValue sci_funcprot(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iFuncprotMode = 0;

    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), "funcprot", 0, 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(77, _("%s: Wrong number of output argument(s): %d expected."), "funcprot", 1);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        iFuncprotMode = ConfigVariable::getFuncprot();
        out.push_back(new types::Double((double)iFuncprotMode));
        return types::Function::OK;
    }

    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "funcprot", 1);
        return types::Function::Error;
    }

    types::Double* pDblIn = in[0]->getAs<types::Double>();

    if (pDblIn->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "funcprot", 1);
        return types::Function::Error;
    }

    iFuncprotMode = (int)pDblIn->get(0);
    if (pDblIn->get(0) != (double)iFuncprotMode)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), "funcprot", 1);
        return types::Function::Error;
    }

    if (iFuncprotMode != 0 && iFuncprotMode != 1 && iFuncprotMode != 2)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: 0, 1 or 2 expected.\n"), "funcprot", 1);
        return types::Function::Error;
    }

    out.push_back(new types::Double((double)ConfigVariable::getFuncprot()));
    ConfigVariable::setFuncprot(iFuncprotMode);
    return types::Function::OK;
}
