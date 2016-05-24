/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
 *
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
/*--------------------------------------------------------------------------*/

#include "function.hxx"
#include "string.hxx"
#include "funcmanager.hxx"
#include "fileio_gw.hxx"

extern "C"
{
#include <stdio.h>
#include <string.h>
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_pathsep(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int dimsArray[2]    = {1, 1};
    wchar_t* wcsSep     = to_wide_string(PATH_SEPARATOR);

    if (in.size() > 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "pathsep", 0);
        FREE(wcsSep);
        return types::Function::Error;
    }

    types::String* pOut = new types::String(2, dimsArray);
    pOut->set(0, wcsSep);
    FREE(wcsSep);
    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
