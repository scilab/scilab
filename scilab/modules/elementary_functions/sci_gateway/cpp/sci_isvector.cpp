/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
* Copyright (C) 2019 - Stéphane Mottelet
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
#include "function.hxx"
#include "arrayof.hxx"
#include "overload.hxx"
#include "bool.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_isvector(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "isvector", 1);
        return types::Function::Error;
    }

    if (out.size() >  1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "isvector", 1);
        return types::Function::Error;
    }

    if (in[0]->isTList() || in[0]->isMList() || in[0]->isGenericType() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_isvector";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }
    else
    {
        types::GenericType *pIn = in[0]->getAs<types::GenericType>();
        out.push_back(new types::Bool(pIn->isVector() && pIn->isScalar() == false));
        return types::Function::OK;
    }
}
