/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Antoine ELIAS
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

#include <string.h>
#include "funcmanager.hxx"
#include "context.hxx"
#include "functions_gw.hxx"
#include "string.hxx"

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_funclist(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    symbol::Context* pContext = symbol::Context::getInstance();
    ;
    if (in.size() > 1)
    {
        return types::Function::Error;
    }

    std::wstring pstLibName;
    if (in.size() == 1)
    {
        types::InternalType* pIT = in[0];

        if (pIT->isString() == false)
        {
            return types::Function::Error;
        }

        types::String *pS = pIT->getAs<types::String>();

        if (pS->getSize() != 1)
        {
            return types::Function::Error;
        }

        pstLibName = pS->get(0);
    }
    else
    {
        pstLibName = L"";
    }

    std::list<symbol::Symbol> funcList;
    int size = pContext->getFunctionList(funcList, pstLibName);

    types::String *pS = new types::String(size, 1);

    std::list<symbol::Symbol>::iterator it;
    int i = 0;
    for (auto it : funcList)
    {
        pS->set(i++, 0, it.getName().c_str());
    }

    out.push_back(pS);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
