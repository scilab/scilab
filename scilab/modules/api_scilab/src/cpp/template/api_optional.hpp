/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#include "gatewaystruct.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(getOptional)(scilabEnv env, scilabOpt opt, const wchar_t* varname)
{
    std::wstring name(varname);
    types::optional_list* l = (types::optional_list*)opt;

    const auto it = l->find(name);
    if (it != l->end())
    {
        return (scilabVar)it->second;
    }

    return NULL;
}
