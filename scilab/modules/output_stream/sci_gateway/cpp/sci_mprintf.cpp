/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) 2010 - DIGITEO - ELIAS Antoine
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

#include "funcmanager.hxx"
#include "output_stream_gw.hxx"
#include "scilab_sprintf.hxx"
#include "scilabWrite.hxx"
#include "function.hxx"
#include "string.hxx"
#include "overload.hxx"

#include <iterator>

extern "C"
{
#include <stdio.h>
#include "Scierror.h"
#include "localization.h"
#include "os_wtoi.h"
}

/*--------------------------------------------------------------------------*/
types::Callable::ReturnValue sci_mprintf(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() < 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: at least %d expected.\n"), "mprintf", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "mprintf" , 1);
        return types::Function::Error;
    }

    for (int i = 1 ; i < in.size() ; i++)
    {
        if (in[i]->isDouble() == false && in[i]->isString() == false)
        {
            std::string stFuncName = "%" + in[i]->getShortTypeStr() + "_mprintf";
            return Overload::call(stFuncName, in, _iRetCount, out);
        }
    }

    int iOutputRows = 0;
    int iNewLine = 0;
    char* pstInput = in[0]->getAs<types::String>()->get()[0];
    char** pstOutput = scilab_sprintf("mprintf", pstInput, in, &iOutputRows, &iNewLine);
    if (pstOutput == NULL)
    {
        //error already set by scilab_sprintf
        return types::Function::Error;
    }

    for (int i = 0 ; i < iOutputRows ; i++)
    {
        if (i)
        {
            scilabForcedWrite("\n");
        }

        scilabForcedWrite(pstOutput[i]);

        fflush(NULL);
        FREE(pstOutput[i]);
    }

    if (iNewLine)
    {
        scilabForcedWrite("\n");
    }

    FREE(pstOutput);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
