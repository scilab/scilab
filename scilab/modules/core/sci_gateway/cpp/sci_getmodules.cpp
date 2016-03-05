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
