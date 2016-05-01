/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
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
#include "windows_tools_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/
const std::string fname = "istssession";
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_istssession(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
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

    /* https://msdn.microsoft.com/en-us/library/ms724385.aspx */
    if (GetSystemMetrics(SM_REMOTESESSION))
    {
        out.push_back(types::Bool::True());
    }
    else
    {
        out.push_back(types::Bool::False());
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

