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

int new_sprintf(const std::string& funcname, const wchar_t* _pwstInput, types::typed_list &in, int* _piOutputRows, int* _piNewLine, wchar_t*** output);

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
            std::wstring wstFuncName = L"%" + in[i]->getShortTypeStr() + L"_mprintf";
            return Overload::call(wstFuncName, in, _iRetCount, out);
        }
    }

    int iOutputRows = 0;
    int iNewLine = 0;
    wchar_t* pwstInput = in[0]->getAs<types::String>()->get()[0];
    wchar_t** pwstOutput = scilab_sprintf("mprintf", pwstInput, in, &iOutputRows, &iNewLine);

    if (pwstOutput == NULL)
    {
        //error already set by scilab_sprintf
        return types::Function::Error;
    }

    for (int i = 0 ; i < iOutputRows ; i++)
    {
        if (i)
        {
            scilabForcedWriteW(L"\n");
        }

        scilabForcedWriteW(pwstOutput[i]);

        fflush(NULL);
        FREE(pwstOutput[i]);
    }

    if (iNewLine)
    {
        scilabForcedWriteW(L"\n");
    }

    FREE(pwstOutput);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
