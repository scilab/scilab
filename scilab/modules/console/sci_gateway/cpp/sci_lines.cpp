/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "console_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_lines(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "lines", 0, 2);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        double size[] = {(double)ConfigVariable::getConsoleWidth(),
                         (double)ConfigVariable::getConsoleLines()
                        };
        types::Double* pDbl = new types::Double(1, 2);
        pDbl->set(size);
        out.push_back(pDbl);
        return types::Function::OK;
    }

    if (in.size() == 2)
    {
        if ((in[1]->isDouble() == false) || !in[1]->getAs<types::Double>()->isScalar())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "lines", 2);
            return types::Function::Error;
        }
        ConfigVariable::setConsoleWidth((int)in[1]->getAs<types::Double>()->get(0));
    }

    if ((in[0]->isDouble() == false) || !in[0]->getAs<types::Double>()->isScalar())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "lines", 1);
        return types::Function::Error;
    }

    ConfigVariable::setConsoleLines((int)in[0]->getAs<types::Double>()->get(0));

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
