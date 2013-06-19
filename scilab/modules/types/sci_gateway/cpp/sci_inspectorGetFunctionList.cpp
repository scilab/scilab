/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "types_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "inspector.hxx"
#include "context.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

using namespace types;

Function::ReturnValue sci_inspectorGetFunctionList(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "inspectorGetFunctionList", 0);
        return Function::Error;
    }

    symbol::Context* pC = symbol::Context::getInstance();

    std::list<symbol::Symbol> FuncName = pC->getFunctionList(L"");

    String* pOut = new String((int)FuncName.size(), 4);

    std::list<symbol::Symbol>::iterator it = FuncName.begin();
    for (int i = 0; it != FuncName.end() ; it++, i++)
    {
        Callable* pCall = pC->get(*it)->getAs<Callable>();
        //Function name
        pOut->set(i, 0, pCall->getName().c_str());
        pOut->set(i, 1, pCall->getModule().c_str());
        pOut->set(i, 2, pCall->getTypeStr().c_str());

        if (pCall->isMacroFile())
        {
            pOut->set(i, 3, pCall->getAs<MacroFile>()->getMacro() == NULL ? L"false" : L"true");
        }
        else
        {
            pOut->set(i, 3, L"");
        }
    }

    out.push_back(pOut);
    return Function::OK;
}
