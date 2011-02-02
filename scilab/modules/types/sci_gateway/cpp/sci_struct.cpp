/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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
#include "arrayof.hxx"
#include "funcmanager.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

Function::ReturnValue sci_struct(typed_list &in, int _piRetCount, typed_list &out)
{
    /* Check number of input arguments: must be even */
    if (in.size()%2 != 0)
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): An even number is expected.\n"), L"struct");
        return Function::Error;
    }

    /* First check if all fields are Strings */
    typed_list::iterator itInput;

    for (itInput = in.begin() ; itInput != in.end() ; itInput = itInput + 2)
    {
        if (!(*itInput)->isString())
        {
            return Function::Error;
        }
    }

    Struct *pRetVal = new Struct();
    InternalType *pFieldValue = NULL;

    for (itInput = in.begin() ; itInput != in.end() ; ++itInput)
    {
        std::wstring psFieldName = std::wstring((*itInput)->getAs<types::String>()->get(0));
        ++itInput;
        pFieldValue = *itInput;
        pRetVal->add(psFieldName, pFieldValue);
    }

    out.push_back(pRetVal);

    return Function::OK;
}
