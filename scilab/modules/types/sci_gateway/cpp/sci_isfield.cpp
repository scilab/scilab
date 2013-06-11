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

#include "types_gw.hxx"
#include "function.hxx"
#include "struct.hxx"
#include "bool.hxx"
#include "string.hxx"

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
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "isfield", 1);
        return Function::Error;
    }

    if (in.size() != 2)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "isfield", 2);
        return Function::Error;
    }

    if (in[0]->isStruct() == false)
    {
        Scierror(999, _("%s:  Wrong type for input argument #%d: struct array expected.\n"), "isfield", 1);
        return Function::Error;
    }

    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s:  Wrong type for input argument #%d: A string expected.\n"), "isfield", 2);
        return Function::Error;
    }

    Struct *pInStruct = in[0]->getAs<Struct>();
    String *pInString = in[1]->getAs<types::String>();

    Bool *pOutBool = new Bool(pInString->getRows(), pInString->getCols());

    for (int i = 0 ; i < pInString->getRows() ; ++i)
    {
        for (int j = 0 ; j < pInString->getCols() ; ++j)
        {
            pOutBool->set(i, j, pInStruct->exists(std::wstring(pInString->get(i, j))));
        }
    }

    out.push_back(pOutBool);

    return Function::OK;
}
