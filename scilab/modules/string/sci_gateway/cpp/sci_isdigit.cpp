/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "string_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "bool.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "isdigit.h"
}

types::Function::ReturnValue sci_isdigit(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pStrIn   = NULL;
    types::Bool* pBOut      = NULL;
    wchar_t* wcstrIn        = NULL;
    int iResultSize         = 0;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "isdigit", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "isdigit", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "isdigit", 1);
        return types::Function::Error;
    }

    pStrIn = in[0]->getAs<types::String>();

    if (pStrIn->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar String expected.\n"), "isdigit", 1);
        return types::Function::Error;
    }

    wcstrIn = pStrIn->get(0);
    int* pbResult = (int*)IsDigitW(wcstrIn, &iResultSize);
    if (pbResult == NULL)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    pBOut = new types::Bool(1, iResultSize);
    pBOut->set(pbResult);
    FREE(pbResult);
    out.push_back(pBOut);

    return types::Function::OK;
}
/*-------------------------------------------------------------------------------------*/
