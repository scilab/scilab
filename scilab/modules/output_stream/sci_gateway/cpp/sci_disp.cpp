/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - ELIAS Antoine
 *  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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
        scilabForcedWriteW(L"\n");
        if (VariableToString(*it, SPACES_LIST) == types::Function::Error)
        {
            return types::Function::Error;
        }
    }

    return types::Function::OK;
}
