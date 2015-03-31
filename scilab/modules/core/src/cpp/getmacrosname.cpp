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
#include "sci_malloc.h"
#include "getmacrosname.h"
#include "charEncoding.h"
}
/*----------------------------------------------------------------------------------*/
char **getMacrosName(int *sizearray)
{
    std::list<std::wstring> macrosList;
    int size = symbol::Context::getInstance()->getMacrosName(macrosList);

    //sort list
    macrosList.sort();
    //same names could come from libraries AND context
    macrosList.unique();

    *sizearray = static_cast<int>(macrosList.size());
    char** macros = NULL;
    if (*sizearray != 0)
    {
        macros = (char**)MALLOC(*sizearray * sizeof(char*));

        int i = 0;
        for (auto it : macrosList)
        {
            macros[i++] = wide_string_to_UTF8(it.c_str());
        }
    }

    return macros;
}
/*----------------------------------------------------------------------------------*/

