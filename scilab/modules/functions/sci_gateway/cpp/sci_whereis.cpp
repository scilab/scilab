/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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

#include "functions_gw.hxx"
#include "context.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C" {
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_whereis(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    /* Check the number of input argument */
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "whereis", 1);
        return types::Function::Error;
    }

    /* Check the number of output argument */
    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "whereis", 1);
        return types::Function::Error;
    }

    if (in[0]->isString())
    {
        types::String* pS = in[0]->getAs<types::String>();

        if (pS->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "whereis", 1);
            return types::Function::Error;
        }

        std::list<std::wstring> lst;
        int size = symbol::Context::getInstance()->getWhereIs(lst, pS->get(0));
        if (lst.empty())
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        types::String* pOut = new types::String(size, 1);
        int i = 0;
        for (std::wstring l : lst)
        {
            pOut->set(i++, l.c_str());
        }

        out.push_back(pOut);
    }
    else
    {
        std::wstring stModule;
        switch (in[0]->getType())
        {
            case types::InternalType::ScilabFunction:
            case types::InternalType::ScilabMacro:
            case types::InternalType::ScilabMacroFile:
                out.push_back(new types::String(in[0]->getAs<types::Callable>()->getModule().c_str()));
                break;
            default:
                out.push_back(types::Double::Empty());
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
