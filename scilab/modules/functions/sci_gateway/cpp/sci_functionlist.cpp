/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Antoine ELIAS
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

using namespace types;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_funclist(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    symbol::Context* pContext = symbol::Context::getInstance();

    wchar_t* pstLibName = NULL;
    if (in.size() > 1)
    {
        return Function::Error;
    }

    if (in.size() == 1)
    {
        InternalType* pIT = in[0];

        if (pIT->isString() == false)
        {
            return Function::Error;
        }

        String *pS = pIT->getAs<types::String>();

        if (pS->getSize() != 1)
        {
            return Function::Error;
        }

        pstLibName = pS->get(0);
    }
    else
    {
        pstLibName = L"";
    }

    wstring libName(pstLibName);
    std::list<symbol::Symbol> FuncList = pContext->getFunctionList(libName, false);

    String *pS = new String((int)FuncList.size(), 1);

    std::list<symbol::Symbol>::iterator it;
    int i = 0;
    for (it = FuncList.begin() ; it != FuncList.end() ; it++)
    {
        pS->set(i++, 0, it->name_get().c_str());
    }

    out.push_back(pS);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
