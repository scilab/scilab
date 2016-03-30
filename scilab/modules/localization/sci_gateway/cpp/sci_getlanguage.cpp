/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#include "localization_gw.hxx"
#include "function.hxx"
#include "string.hxx"


extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
#include "setgetlanguage.h"
}

types::Function::ReturnValue sci_getlanguage(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "getlanguage", 0);
        return types::Function::Error;
    }

    if (_piRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "getlanguage", 1);
        return types::Function::Error;
    }

    wchar_t* pwstLang = getlanguage();
    out.push_back(new types::String(pwstLang));
    free(pwstLang);

    return types::Function::OK;
}
