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
/*--------------------------------------------------------------------------*/
#include "polynomials_gw.hxx"
#include "function.hxx"
#include "bool.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_simpMode(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "simp_mode", 0, 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "simp_mode", 1);
        return types::Function::Error;
    }

    // set simp mode
    if (in.size() == 1)
    {
        if (in[0]->isBool() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), "simp_mode", 1);
            return types::Function::Error;
        }

        types::Bool* pBIn = in[0]->getAs<types::Bool>();

        if (pBIn->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A boolean scalar expected.\n"), "simp_mode", 1);
            return types::Function::Error;
        }

        ConfigVariable::setSimpMode(pBIn->get(0));
    }
    else // if (in.size() == 0) get simp mode
    {
        out.push_back(new types::Bool(ConfigVariable::getSimpMode()));
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

