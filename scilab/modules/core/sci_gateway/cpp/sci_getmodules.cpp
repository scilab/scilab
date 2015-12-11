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
/*--------------------------------------------------------------------------*/

#include <list>

#include "core_gw.hxx"
#include "configvariable.hxx"
#include "string.hxx"
#include "function.hxx"

types::Function::ReturnValue sci_getmodules(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 0 || _iRetCount > 1)
    {
        return types::Function::Error;
    }


    std::list<std::wstring> sModuleList = ConfigVariable::getModuleList();
    types::String *pOut = new types::String(static_cast<int>(sModuleList.size()), 1);

    std::list<std::wstring>::iterator it = sModuleList.begin();
    for (int i = 0; it != sModuleList.end() ; it++, i++)
    {
        pOut->set(i, it->c_str());
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
