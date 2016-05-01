/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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
#include "string.hxx"
#include "double.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "charEncoding.h"
#include "localization.h"
#include "Scierror.h"
}

static const char fname[] = "recursionLimit";

types::Function::ReturnValue sci_recursionlimit(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), fname, 0, 1);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        out.push_back(new types::Double(ConfigVariable::getRecursionLimit()));
        return types::Function::OK;
    }

    if (in[0]->isString())
    {
        types::String* s = in[0]->getAs<types::String>();
        if (s->isScalar() == false || wcscmp(s->get()[0], L"current") != 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' expected.\n"), fname, 1, "current");
            return types::Function::Error;
        }

        out.push_back(new types::Double(ConfigVariable::getRecursionLevel()));
        return types::Function::OK;
    }

    if (in[0]->isDouble())
    {
        types::Double* d = in[0]->getAs<types::Double>();
        if (d->isScalar() == false || d->get()[0] < 10)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: A value >= %d expected.\n"), fname, 1, 10);
            return types::Function::Error;
        }

        out.push_back(new types::Double(ConfigVariable::getRecursionLimit()));
        ConfigVariable::setRecursionLimit(static_cast<int>(d->get()[0]));
        return types::Function::OK;
    }

    Scierror(999, _("%s: Wrong type for input argument #%d: String or integer expected.\n"), fname, 1);
    return types::Function::Error;

}