/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "windows_tools_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "FindFileAssociation.h"
#include "PATH_MAX.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
const std::string fname = "findfileassociation";
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_findfileassociation(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pS = nullptr;
    std::string param1;
    std::string param2("open");

    int rhs = static_cast<int>(in.size());
    if (rhs != 1 && rhs != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), fname.data(), 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    pS = in[0]->getAs<types::String>();
    if (pS->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    param1 = pS->get()[0];

    if (rhs == 2)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname.data(), 2);
            return types::Function::Error;
        }

        pS = in[1]->getAs<types::String>();
        if (pS->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), fname.data(), 2);
            return types::Function::Error;
        }

        param2 = pS->get()[0];
    }

    char* output = FindFileAssociation(param1.data(), param2.data()) ;
    if (output)
    {
        out.push_back(new types::String(output));
    }
    else
    {
        out.push_back(types::Double::Empty());
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
