/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2011 - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/

#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include "dynamic_link.h"
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_c_link(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iLib = -1;
    char* pstFunctionName = NULL;

    if(in.size() < 1 || in.size() > 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"c_link", 1, 2);
        return types::Function::Error;
    }

    if(_iRetCount > 2)
    {
        ScierrorW(78,_W("%ls: Wrong number of output argument(s): %d to %d expected.\n"), L"c_link", 1, 2);
        return types::Function::Error;
    }

    if(in.size() == 2)
    {
        types::Double* pDId = in[1]->getAs<types::Double>();
        if(pDId == NULL || pDId->isScalar() == false)
        {
            Scierror(999 ,_("%s : second argument must be a unique id of a shared library.\n"), L"c_link");
            return types::Function::Error;
        }

        iLib = (int)pDId->get(0);
    }


    types::String* pSLibName = in[0]->getAs<types::String>();
    if(pSLibName == NULL || pSLibName->isScalar() == false)
    {
        ScierrorW(999 ,_W("%ls : Wrong type for input argument #%d: A string expected.\n"), L"c_link", 1);
        return types::Function::Error;
    }

    pstFunctionName = wide_string_to_UTF8(pSLibName->get(0));
    BOOL bFind = c_link(pstFunctionName, &iLib);
    FREE(pstFunctionName);

    out.push_back(new types::Bool(bFind));
    if(_iRetCount == 2)
    {
        out.push_back(new types::Double(iLib));
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
