/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Digiteo 2011 - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "double.hxx"
#include "funcmanager.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
}


types::Function::ReturnValue sci_strtod(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pOutDouble = NULL;
    types::String* pOutString = NULL;
    types::String* pString = NULL;

    if(in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "strtod", 1);
        return types::Function::Error;
    }
    if(_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "strtod", 1, 2);
        return types::Function::Error;
    }

    pString = in[0]->getAs<types::String>();

    if(_iRetCount == 2)
    {
        pOutDouble = new types::Double(pString->getDims(),pString->getDimsArray());
        pOutString = new types::String(pString->getDims(),pString->getDimsArray());

        for (int i = 0 ; i < pString->getSize() ; i++)
        {
            wchar_t *stopstring = NULL;
            pOutDouble->set(i,wcstod(pString->get(i),&stopstring));
            if(stopstring)
            {
                pOutString->set(i,stopstring);
            }
            else
            {
                pOutString->set(i,L"");
            }
        }

        out.push_back(pOutDouble);
        out.push_back(pOutString);
    }
    else // _iRetCount == 1
    {
        pOutDouble = new types::Double(pString->getDims(),pString->getDimsArray());

        for (int i = 0 ; i < pString->getSize() ; i++)
        {
            wchar_t *stopstring = NULL;
            pOutDouble->set(i,wcstod(pString->get(i),&stopstring));
        }

        out.push_back(pOutDouble);
    }

    return types::Function::OK;
}

