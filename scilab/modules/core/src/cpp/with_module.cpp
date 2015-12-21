/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <list>
#include <string>
#include "configvariable.hxx"

extern "C"
{
#include <string.h>
#include "with_module.h"
#include "getmodules.h"
}
/*--------------------------------------------------------------------------*/
BOOL with_module(wchar_t* _pwstModule)
{
    if (_pwstModule)
    {
        std::list<std::wstring> sModuleList = ConfigVariable::getModuleList();
        std::list<std::wstring>::const_iterator it;
        for (it = sModuleList.begin() ; it != sModuleList.end() ; it++)
        {
            if (wcscmp((*it).c_str(), _pwstModule) == 0)
            {
                return TRUE;
            }
        }
    }
    return FALSE;

}
/*--------------------------------------------------------------------------*/
