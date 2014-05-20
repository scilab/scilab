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
#include "getmacrosname.h"
#include "charEncoding.h"
}
/*----------------------------------------------------------------------------------*/
char **getMacrosName(int *sizearray)
{
    std::list<std::wstring>* plMacrosList = symbol::Context::getInstance()->getMacrosName();

    //sort list
    plMacrosList->sort();
    //same names could come from libraries AND context
    plMacrosList->unique();

    *sizearray = (int)plMacrosList->size();
    char** macros = NULL;
    if (*sizearray != 0)
    {
        macros = (char**)MALLOC(*sizearray * sizeof(char*));

        std::list<std::wstring>::iterator it = plMacrosList->begin();
        for (int i = 0; it != plMacrosList->end(); ++it, i++)
        {
            macros[i] = wide_string_to_UTF8((*it).c_str());
        }

        delete plMacrosList;
    }

    return macros;
}
/*----------------------------------------------------------------------------------*/

