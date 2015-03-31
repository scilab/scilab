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
#include "getvariablesname.h"
#include "charEncoding.h"
}
/*----------------------------------------------------------------------------------*/
char **getVariablesName(int *sizearray, BOOL sorted)
{
    std::list<std::wstring> varNames;
    *sizearray = symbol::Context::getInstance()->getVarsName(varNames);
    char** variables = NULL;

    if (*sizearray != 0)
    {
        variables = (char**)MALLOC(*sizearray * sizeof(char*));

        if (sorted)
        {
            varNames.sort();
        }

        int i = 0;
        for (auto it : varNames)
        {
            variables[i++] = wide_string_to_UTF8(it.c_str());
        }
    }

    return variables;
}

