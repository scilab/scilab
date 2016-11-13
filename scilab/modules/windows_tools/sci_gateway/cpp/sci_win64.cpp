/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2011 - Allan CORNET
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

#include "windows_tools_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
const std::string fname = "win64";
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_win64(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int status = 0;
    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname.data(), 0);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

#ifdef _WIN64
    status = 1;
#endif

    out.push_back(new types::Bool(status));
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

