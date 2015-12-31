/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
 *
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    if (in.size() > 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "pathsep", 0);
        return types::Function::Error;
    }

    out.push_back(new types::String(PATH_SEPARATOR));
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
