/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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

#include "context.hxx"

extern "C" {
#include "sci_malloc.h"
#include "getfunctionsname.h"
#include "charEncoding.h"
}
/*----------------------------------------------------------------------------------*/
char **getFunctionsName(int *sizearray)
{
    std::list<std::wstring> macrosList;
    *sizearray = symbol::Context::getInstance()->getFunctionsName(macrosList);

    char** functions = NULL;
    if (*sizearray != 0)
    {
        functions = (char**)MALLOC(*sizearray * sizeof(char*));

        macrosList.sort();

        int i = 0;
        for (auto it : macrosList)
        {
            functions[i++] = wide_string_to_UTF8(it.c_str());
        }
    }

    return functions;
}
