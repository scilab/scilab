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
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_varn(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Polynom* pPolyIn  = NULL;
    types::String* pStrName  = NULL;
    types::Polynom* pPolyOut = NULL;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "varn", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "varn", 1);
        return types::Function::Error;
    }

    if (in[0]->isPoly() == false)
    {
        if (in[0]->isDouble() && in[0]->getAs<types::Double>()->isEmpty())
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_varn";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pPolyIn = in[0]->getAs<types::Polynom>();

    if (in.size() == 1)
    {
        out.push_back(new types::String(pPolyIn->getVariableName().c_str()));
    }
    else // if (in.size() == 2)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "varn", 2);
            return types::Function::Error;
        }

        pStrName = in[1]->getAs<types::String>();
        if (pStrName->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), "varn", 2);
            return types::Function::Error;
        }

        std::wstring wstrName = pStrName->get(0);
        // search blank
        size_t blankpos = wstrName.find_first_of(L" ");
        if ((int)blankpos != -1)
        {
            // blank found
            Scierror(999, _("%s: Wrong value for input argument #%d : Valid variable name expected.\n"), "varn", 2);
            return types::Function::Error;
        }

        pPolyOut = pPolyIn->clone()->getAs<types::Polynom>();
        pPolyOut->setVariableName(std::wstring(wstrName));
        out.push_back(pPolyOut);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

