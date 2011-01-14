/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "function.hxx"
#include "struct.hxx"
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

Function::ReturnValue sci_isfield(typed_list &in, int _iRetCount, typed_list &out)
{
    if (_iRetCount != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"isfield", 1);
        return Function::Error;
    }

    if (in.size() != 2)
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"isfield", 2);
        return Function::Error;
    }

    if(in[0]->isStruct() == false)
    {
        ScierrorW(999, _W("%ls:  Wrong type for input argument #%d: struct array expected.\n"), L"isfield", 1);
        return Function::Error;
    }

    if(in[1]->isString() == false)
    {
        ScierrorW(999, _W("%ls:  Wrong type for input argument #%d: A string expected.\n"), L"isfield", 2);
        return Function::Error;
    }

    Struct *pInStruct = in[0]->getAs<Struct>();
    String *pInString = in[1]->getAs<String>();

    Bool *pOutBool = new Bool(pInString->rows_get(), pInString->cols_get());

    for (int i = 0 ; i < pInString->rows_get() ; ++i)
    {
        for(int j = 0 ; j < pInString->cols_get() ; ++j)
        {
            pOutBool->bool_set(i, j, pInStruct->exists(std::wstring(pInString->string_get(i,j))));
        }
    }

    out.push_back(pOutBool);

    return Function::OK;
}
