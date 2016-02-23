/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#include "core_gw.hxx"
#include "configvariable.hxx"
#include "double.hxx"
#include "string.hxx"
#include "function.hxx"
#include "context.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
bool isValidName(wchar_t* _pwstName)
{
    if (_pwstName == NULL)
    {
        return false;
    }

    if (isdigit(_pwstName[0]))
    {
        return false;
    }

    int size = (int)wcslen(_pwstName);
    for (int i = 1 ; i < size ; ++i)
    {
        wchar_t c = _pwstName[i];
        if (c != L'_' && c != L'?' && c != L'!' && isalnum(c) == false)
        {
            return false;
        }

    }
    return true;
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_newfun(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "newfun" , 2);
        return types::Function::Error;
    }

    types::InternalType* pIT1 = in[0];
    types::InternalType* pIT2 = in[1];

    if (pIT1->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "newfun", 1);
        return types::Function::Error;
    }

    types::String* pS1 = pIT1->getAs<types::String>();
    if (pS1->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), "newfun", 1);
        return types::Function::Error;
    }

    wchar_t* pwcsNewName = pS1->get(0);

    //check is a valid name
    if (isValidName(pwcsNewName) == false)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Valid function name expected.\n"), "newfun", 1);
        return types::Function::Error;
    }

    if (pIT2->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "newfun", 2);
        return types::Function::Error;
    }

    types::String* pS2 = pIT2->getAs<types::String>();
    if (pS2->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), "newfun", 2);
        return types::Function::Error;
    }

    wchar_t* pwcsName = pS2->get(0);

    types::Function* pFunc = NULL;
    symbol::Context* pCtx = symbol::Context::getInstance();

    symbol::Variable* pVar = pCtx->getOrCreate(symbol::Symbol(pwcsName));
    symbol::Variable::StackVar stack;

    //get original function

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
            pFunc = pSV->m_pIT->getAs<types::Function>();
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

    if (pFunc == NULL)
    {
        Scierror(999, _("%s: function-name is incorrect.\n"), "newfun");
        return types::Function::Error;
    }

    //new function
    pVar = pCtx->getOrCreate(symbol::Symbol(pwcsNewName));
    if (pVar->empty())
    {
        pVar->put(pFunc, 0);
    }
    else
    {
        //unstack all elements and stack them in new stack ( reverse order )
        while (pVar->empty() == false)
        {
            stack.push(pVar->top());
            pVar->pop();
        }

        symbol::ScopedVariable* pSV = stack.top();
        if (pSV->m_iLevel == 0)
        {
            stack.pop();
            //clear current var and insert new one
            types::InternalType* pIT = pSV->m_pIT;
            pIT->DecreaseRef();
            pIT->killMe();
        }

        pVar->put(pFunc, 0);

        //move all elements at orginal place and order
        while (stack.empty() == false)
        {
            pSV = stack.top();
            stack.pop();
            //pSV->m_pIT->DecreaseRef();
            pVar->put(pSV);
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
