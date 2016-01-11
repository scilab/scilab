/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - ELIAS Antoine
 *  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "output_stream_gw.hxx"
#include "function.hxx"
#include "scilabWrite.hxx"
#include "types_tools.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_disp(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::typed_list::reverse_iterator it;

    if (in.empty())
    {
        Scierror(999, _("%s: Wrong number of input arguments: At least %d expected.\n"), "disp", 1);
        return types::Function::Error;
    }

    for (it = in.rbegin() ; it != in.rend() ; it++)
    {
        scilabForcedWrite("\n");
        if (VariableToString(*it, SPACES_LIST) == types::Function::Error)
        {
            return types::Function::Error;
        }
    }

    return types::Function::OK;
}
