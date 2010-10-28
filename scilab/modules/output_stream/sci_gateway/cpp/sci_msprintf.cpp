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

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_msprintf(typed_list &in, int _piRetCount, typed_list &out)
{
    //Structure to store, link between % and input value
    ArgumentPosition* pArgs = NULL;

    if(in.size() < 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: at least %d expected.\n"), L"msprintf", 1);
        return Function::Error;
    }

    if(in[0]->isString() == false || in[0]->getAsString()->size_get() != 1)
    {
        ScierrorW(999, _W("l%s: Wrong type for input argument #%d: A string expected.\n"), L"msprintf" ,1);
        return Function::Error;
    }

    for(int i = 1 ; i < in.size() ; i++)
    {
        if(in[i]->isDouble() == false && in[i]->isString() == false)
        {
            //TODO: Overload
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Real matrix or matrix of strings expected.\n"), L"msprintf", i + 1);
            return Function::Error;
        }
    }

    wchar_t* pwstInput = in[0]->getAsString()->string_get()[0];
    int iNumberPercent = 0;
    for(int i = 0 ; i < wcslen(pwstInput) ; i++)
    {
        if(pwstInput[i] == L'%')
        {
			iNumberPercent++;
			if (pwstInput[i + 1] == L'%')
			{
                //it is a %%, not a %_ 
				iNumberPercent--;
                //force incremantation to bypass the second % of %%
                i++;
			}
        }
    }

    //Input values must be less or equal than excepted
    if((in.size() - 1) > iNumberPercent)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: at most %d expected.\n"), L"msprintf", iNumberPercent);
        return Function::Error;
    }

    //determine if imput values are ... multiple values
    int iNumberCols = 0;
    if( in.size() > 1 )
    {
        int iRefRows = in[1]->getAsGenericType()->rows_get();
        for(int i = 1 ; i < in.size() ; i++)
        {
            //all arguments must have the same numbers of rows !
            if(iRefRows != in[i]->getAsGenericType()->rows_get())
            {
                ScierrorW(999, _W("%ls: Wrong number of input arguments: data doesn't fit with format.\n"), L"msprintf");
                return Function::Error;
            }

            iNumberCols += in[i]->getAsGenericType()->cols_get();
        }
    }

    if(iNumberCols != iNumberPercent)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: data doesn't fit with format.\n"), L"msprintf");
        return Function::Error;
    }


    //fill ArgumentPosition structure
    pArgs = new ArgumentPosition[iNumberPercent];
    int idx = 0;
    for(int i = 1 ; i < in.size() ; i++)
    {
        for(int j = 0 ; j < in[i]->getAsGenericType()->cols_get() ; j++)
        {
            pArgs[idx].iArg = i;
            pArgs[idx].iPos = j;
            pArgs[idx].type = in[i]->getType();
            idx++;
        }
    }

    int iOutputRows = 0;
    wchar_t** pwstOutput = scilab_sprintf(L"msprintf", pwstInput, in, pArgs, iNumberPercent, &iOutputRows);

    String* pOut = new String(iOutputRows, 1);
    pOut->string_set(pwstOutput);
    out.push_back(pOut);

    for(int i = 0 ; i < iOutputRows ; i++)
    {
        FREE(pwstOutput[i]);
    }
    FREE(pwstOutput);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/ 
