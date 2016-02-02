/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) 2010 - DIGITEO - ELIAS Antoine
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

#include "funcmanager.hxx"
#include "output_stream_gw.hxx"
#include "scilab_sprintf.hxx"
#include "function.hxx"
#include "string.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
}

/*--------------------------------------------------------------------------*/
types::Callable::ReturnValue sci_msprintf(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() < 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: at least %d expected.\n"), "msprintf", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "msprintf" , 1);
        return types::Function::Error;
    }

    for (int i = 1 ; i < in.size() ; i++)
    {
        if (in[i]->isDouble() == false && in[i]->isString() == false)
        {
            std::wstring wstFuncName = L"%" + in[i]->getShortTypeStr() + L"_msprintf";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }
    }

    int iOutputRows = 0;
    int iNewLine = 0;
    wchar_t* pwstInput = in[0]->getAs<types::String>()->get()[0];
    wchar_t** pwstOutput = scilab_sprintf("msprintf", pwstInput, in, &iOutputRows, &iNewLine);

    if (pwstOutput == NULL)
    {
        return types::Function::Error;
    }

    types::String* pOut = new types::String(iOutputRows, 1);
    pOut->set(pwstOutput);
    freeArrayOfWideString(pwstOutput, iOutputRows);
    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
