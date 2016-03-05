/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
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
#include "charEncoding.h"
#include "localization.h"
#include "Scierror.h"
}

types::Function::ReturnValue sci_ieee(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double pdblIeee = 0;

    // *** check number of input args. ***
    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "ieee", 0, 1);
        return types::Function::Error;
    }

    // *** check number of output args. ***
    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "ieee", 1);
        return types::Function::Error;
    }

    // *** get or set ieee. ***
    if (in.size() == 0)
    {
        out.push_back(new types::Double((double)ConfigVariable::getIeee()));
    }
    else
    {
        if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A scalar expected.\n"), "ieee", 1);
            return types::Function::Error;
        }

        pdblIeee = in[0]->getAs<types::Double>()->get(0);

        if (pdblIeee < 0 || pdblIeee > 2)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d : 0, 1 or 2 expected.\n"), "ieee", 1);
            return types::Function::Error;
        }

        ConfigVariable::setIeee((int)pdblIeee);
    }

    return types::Function::OK;
}
