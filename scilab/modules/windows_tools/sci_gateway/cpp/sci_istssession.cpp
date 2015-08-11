/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

