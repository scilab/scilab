/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>
#include "funcmanager.hxx"
#include "context.hxx"
#include "functions_gw.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

using namespace types;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_librarieslist(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "librarieslist", 0);
        return Function::Error;
    }

    std::list<std::wstring>* lst = symbol::Context::getInstance()->getLibrariesList();

    String* pOut = new String(static_cast<int>(lst->size()), 1);
    int i = 0;
    for (auto l : *lst)
    {
        pOut->set(i++, l.c_str());
    }

    delete lst;
    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
