/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "context.hxx"

extern "C" {
#include "MALLOC.h"
#include "getfunctionsname.h"
}
/*----------------------------------------------------------------------------------*/
char **getFunctionsName(int *sizearray)
{
    std::list<std::wstring> plMacrosList = symbol::Context::getInstance()->getFunctionsName();
    std::list<std::wstring>::iterator it;
    *sizearray = (int)plMacrosList.size();
    char** functions = (char**)MALLOC(*sizearray * sizeof(char*));

    plMacrosList.sort();

    int i = 0;
    for (it = plMacrosList.begin(); it != plMacrosList.end(); it++, i++)
    {
        functions[i] = wide_string_to_UTF8((*it).c_str());
    }

    return functions;
}
