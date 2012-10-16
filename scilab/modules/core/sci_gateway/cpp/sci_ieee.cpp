/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"ieee", 0, 1);
        return types::Function::Error;
    }

    // *** check number of output args. ***
    if (_iRetCount > 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"ieee", 1);
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
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A scalar expected.\n"), L"ieee", 1);
            return types::Function::Error;
        }

        pdblIeee = in[0]->getAs<types::Double>()->get(0);

        if (pdblIeee < 0 || pdblIeee > 2)
        {
            ScierrorW(999, _W("%ls: Wrong value for input argument #%d : 0, 1 or 2 expected.\n"), L"ieee", 1);
            return types::Function::Error;
        }

        ConfigVariable::setIeee((int)pdblIeee);
    }

    return types::Function::OK;
}
