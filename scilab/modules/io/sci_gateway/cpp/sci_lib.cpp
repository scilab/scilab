/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#include <string.h>
#include "function.hxx"
#include "io_gw.hxx"
#include "string.hxx"
#include "library.hxx"
#include "loadlib.hxx"

extern "C"
{
#include "Scierror.h"
#include "sci_malloc.h"
#include "pathconvert.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_lib(types::typed_list &in, int /*_iRetCount*/, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(78, _("%s: Wrong number of input argument(s): %d expected.\n"), "lib", 1);
        return types::Function::Error;
    }

    types::InternalType* pIT = in[0];

    if (pIT->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "lib", 1);
        return types::Function::Error;
    }

    types::String *pS = pIT->getAs<types::String>();

    if (pS->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), "lib", 1);
        return types::Function::Error;
    }

    wchar_t* pstPath = pS->get(0);
    wchar_t* pwstPath = pathconvertW(pstPath, TRUE, TRUE, AUTO_STYLE);
    int err = 0;
    types::Library* lib = loadlib(pwstPath, &err, false, false);
    FREE(pwstPath);

    switch (err)
    {
        case 0 :
            //no error
            break;
        case 1:
        {
            char* pst = wide_string_to_UTF8(pstPath);
            Scierror(999, _("%s: %s is not a valid lib path.\n"), "lib", pst);
            FREE(pst);
            return types::Function::Error;
        }
        case 2:
        {
            Scierror(999, "%s: %s", "lib", _("Redefining permanent variable.\n"));
            return types::Function::Error;
        }
        case 3:
        {
            Scierror(999, "%s: %s", "lib", _("Cannot read lib file: Not in proper XML format.\n"));
            return types::Function::Error;
        }
        case 4:
        {
            Scierror(999, "%s: %s", "lib", _("Old binary lib detected. Please recompile it for Scilab 6.\n"));
            return types::Function::Error;
        }
        default:
        {
            //nothing
        }
    }

    out.push_back(lib);
    return types::Function::OK;
}


/*--------------------------------------------------------------------------*/
