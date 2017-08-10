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

