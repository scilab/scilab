/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "function.hxx"
#include "callable.hxx"
#include "context.hxx"
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

types::Function::ReturnValue sci_clearfun(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "clearfun" , 1);
        types::Function::Error;
    }

    types::InternalType* pIT1 = in[0];
    if (pIT1->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "clearfun", 1);
        types::Function::Error;
    }

    types::String* pS1 = pIT1->getAs<types::String>();
    if (pS1->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), "clearfun", 1);
        types::Function::Error;
    }

    wchar_t* pwcsName = pS1->get(0);

    bool bDeleted = false;
    symbol::Context* pCtx = symbol::Context::getInstance();

    symbol::Variable* pVar = pCtx->getOrCreate(symbol::Symbol(pwcsName));
    symbol::Variable::StackVar stack;

    //unstack all elements and stack them in new stack ( reverse order )
    while (pVar->empty() == false)
    {
        stack.push(pVar->top());
        pVar->pop();
    }

    if (stack.empty() == false)
    {
        symbol::ScopedVariable* pSV = stack.top();
        if (pSV->m_iLevel == 0 && pSV->m_pIT->isFunction())
        {
            //remove
            pSV->m_pIT->DecreaseRef();
            pSV->m_pIT->killMe();
            stack.pop();
            bDeleted = true;
            delete pSV;
        }

        //move all elements at orginal place and order
        while (stack.empty() == false)
        {
            pSV = stack.top();
            stack.pop();
            //pSV->m_pIT->DecreaseRef();
            pVar->put(pSV);
        }
    }

    out.push_back(new types::Bool(bDeleted ? 1 : 0));
    types::Function::OK;
}
