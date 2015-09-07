/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) 2010 - DIGITEO - ELIAS Antoine
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "funcmanager.hxx"
#include "output_stream_gw.hxx"
#include "scilab_sprintf.hxx"
#include "scilabWrite.hxx"
#include "function.hxx"
#include "string.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include <iterator>

extern "C"
{
#include <stdio.h>
#include "Scierror.h"
#include "localization.h"
#include "os_wtoi.h"
}

int new_sprintf(const std::string& funcname, const wchar_t* _pwstInput, typed_list &in, int* _piOutputRows, int* _piNewLine, wchar_t*** output);

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
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "mprintf" , 1);
        return types::Function::Error;
    }

    for (int i = 1 ; i < in.size() ; i++)
    {
        if (in[i]->isDouble() == false && in[i]->isString() == false)
        {
            ast::ExecVisitor exec;
            std::wstring wstFuncName = L"%" + in[i]->getShortTypeStr() + L"_mprintf";
            return Overload::call(wstFuncName, in, _iRetCount, out, &exec);
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