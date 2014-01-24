/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "data_structures_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "tlist.hxx"
#include "double.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*-----------------------------------------------------------------------------------*/
types::Function::ReturnValue sci_rlist(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if(in.size() == 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): At most %d expected.\n"), "rlist", 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "rlist", 1);
        return types::Function::Error;
    }

    types::TList* pTLOut = new types::TList();
    wchar_t* wcsFields[4] = {L"r", L"num", L"den", L"dt"};
    types::String* pStr = new types::String(1, 4, wcsFields);
    pTLOut->set(0, pStr);

    for(int i = 0; i < in.size(); i++)
    {
        pTLOut->set(i+1, in[i]);
    }

    // set dt
    if(in.size() == 2)
    {
        pTLOut->set(3, types::Double::Empty());
    }

    out.push_back(pTLOut);
    return types::Function::OK;
}
