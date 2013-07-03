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
#include "getvariablesname.h"
}
/*----------------------------------------------------------------------------------*/
char **getVariablesName(int *sizearray, BOOL sorted)
{
    std::list<std::wstring> plVarNames = symbol::Context::getInstance()->getVarsName();
    std::list<std::wstring>::iterator it;
    *sizearray = (int)plVarNames.size();
    char** variables = (char**)MALLOC(*sizearray * sizeof(char*));

    if (sorted)
    {
        plVarNames.sort();
    }

    int i = 0;
    for (it = plVarNames.begin(); it != plVarNames.end(); it++, i++)
    {
        variables[i] = wide_string_to_UTF8((*it).c_str());
    }

    return variables;
}

