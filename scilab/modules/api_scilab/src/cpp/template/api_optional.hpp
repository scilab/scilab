/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
