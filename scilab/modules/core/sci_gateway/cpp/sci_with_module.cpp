/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "funcmanager.hxx"
#include "function.hxx"
#include "string.hxx"
#include "bool.hxx"
#include "configvariable.hxx"


extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_with_module(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pStr = NULL;
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "with_module", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "with_module", 1);
        return types::Function::Error;
    }

    pStr = in[0]->getAs<types::String>();
    if (pStr->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), "with_module", 1);
        return types::Function::Error;
    }

    wchar_t* pwstModuleName = pStr->get(0);

    types::Bool* pOut = new types::Bool(0);
    std::list<std::wstring> sModuleList = ConfigVariable::getModuleList();
    std::list<std::wstring>::iterator it;
    for (it = sModuleList.begin() ; it != sModuleList.end() ; it++)
    {
        if (*it == pwstModuleName)
        {
            pOut->get()[0] = 1;
            break;
        }
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/


