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

extern "C"
{
#include <stdio.h>
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Callable::ReturnValue sci_mprintf(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //Structure to store, link between % and input value
    ArgumentPosition* pArgs = NULL;

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
            std::wstring wstFuncName = L"%"  + in[i]->getShortTypeStr() + L"_mprintf";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
        }
    }

    wchar_t* pwstInput = in[0]->getAs<types::String>()->get()[0];
    int iNumberPercent = 0;
    int iNumberPercentPercent = 0;
    for (int i = 0 ; i < wcslen(pwstInput) ; i++)
    {
        if (pwstInput[i] == L'%')
        {
            iNumberPercent++;
            if (pwstInput[i + 1] == L'%')
            {
                //it is a %%, not a %_
                iNumberPercentPercent++;
                //force incremantation to bypass the second % of %%
                i++;
            }
        }
    }

    //Input values must be less or equal than excepted
    if ((in.size() - 1) > iNumberPercent - iNumberPercentPercent)
    {
        Scierror(999, _("%s: Wrong number of input arguments: at most %d expected.\n"), "mprintf", iNumberPercent);
        return types::Function::Error;
    }

    //determine if imput values are ... multiple values
    int iNumberCols = 0;
    if ( in.size() > 1 )
    {
        int iRefRows = in[1]->getAs<GenericType>()->getRows();
        for (int i = 1 ; i < in.size() ; i++)
        {
            //all arguments must have the same numbers of rows !
            if (iRefRows != in[i]->getAs<GenericType>()->getRows())
            {
                Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), "mprintf");
                return types::Function::Error;
            }

            iNumberCols += in[i]->getAs<GenericType>()->getCols();
        }
    }

    if (iNumberCols != iNumberPercent - iNumberPercentPercent)
    {
        Scierror(999, _("%s: Wrong number of input arguments: data doesn't fit with format.\n"), "mprintf");
        return types::Function::Error;
    }


    //fill ArgumentPosition structure
    pArgs = new ArgumentPosition[iNumberPercent - iNumberPercentPercent];
    int idx = 0;
    for (int i = 1 ; i < in.size() ; i++)
    {
        for (int j = 0 ; j < in[i]->getAs<GenericType>()->getCols() ; j++)
        {
            pArgs[idx].iArg = i;
            pArgs[idx].iPos = j;
            pArgs[idx].type = in[i]->getType();
            idx++;
        }
    }

    int iOutputRows = 0;
    int iNewLine = 0;
    wchar_t** pwstOutput = scilab_sprintf(L"mprintf", pwstInput, in, pArgs, iNumberPercent, &iOutputRows, &iNewLine);

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
    delete[] pArgs;
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
