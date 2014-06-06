/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - ELIAS Antoine
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "function.hxx"
#include "funcmanager.hxx"
#include "scilabWrite.hxx"
#include "configvariable.hxx"
#include "output_stream_gw.hxx"
#include "string.hxx"
#include "tlist.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "context.hxx"
#include "symbol.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

using namespace types;

Function::ReturnValue sci_disp(typed_list &in, int _iRetCount, typed_list &out)
{
    typed_list::reverse_iterator it;

    if (in.empty())
    {
        Scierror(999, _("%s: Wrong number of input arguments: At least %d expected.\n"), "disp", 1);
        return Function::Error;
    }

    for (it = in.rbegin() ; it != in.rend() ; it++)
    {
        bool isTList = (*it)->getType() == GenericType::RealTList;
        bool isMList = (*it)->getType() == GenericType::RealMList;

        scilabForcedWriteW(L"\n");
        if (isTList || isMList)
        {
            wchar_t* wcsStr = NULL;
            if (isTList)
            {
                TList* pTL = (*it)->getAs<TList>();
                wcsStr = pTL->get(0)->getAs<String>()->get(0);
            }
            else
            {
                MList* pML = (*it)->getAs<MList>();
                wcsStr = pML->get(0)->getAs<String>()->get(0);
            }

            typed_list input;
            input.push_back(*it);
            std::wstring wstFuncName = L"%"  + std::wstring(wcsStr) + L"_p";
            if (symbol::Context::getInstance()->get(symbol::Symbol(wstFuncName)))
            {
                return Overload::call(wstFuncName, input, 1, out, new ast::ExecVisitor());
            }
        }

        std::wostringstream ostr;
        (*it)->toString(ostr);
        scilabForcedWriteW(ostr.str().c_str());
    }

    return Function::OK;
}
