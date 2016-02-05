/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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
#include "function.hxx"
#include "string.hxx"
#include "macrofile.hxx"
#include "inspector.hxx"
#include "context.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_inspectorGetFunctionList(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "inspectorGetFunctionList", 0);
        return types::Function::Error;
    }

    symbol::Context* pC = symbol::Context::getInstance();

    std::list<symbol::Symbol> funcName;
    int size = pC->getFunctionList(funcName, L"");

    types::String* pOut = new types::String(size, 4);

    int i = 0;
    for (auto it : funcName)
    {
        types::Callable* pCall = pC->get(it)->getAs<types::Callable>();
        //Function name
        pOut->set(i, 0, pCall->getName().c_str());
        pOut->set(i, 1, pCall->getModule().c_str());
        pOut->set(i, 2, pCall->getTypeStr().c_str());

        if (pCall->isMacroFile())
        {
            pOut->set(i, 3, pCall->getAs<types::MacroFile>()->getMacro() == NULL ? L"false" : L"true");
        }
        else
        {
            pOut->set(i, 3, L"");
        }

        ++i;
    }

    out.push_back(pOut);
    return types::Function::OK;
}
