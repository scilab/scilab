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

#include "alltypes.hxx"
#include "funcmanager.hxx"

using namespace types;

Function::ReturnValue sci_struct(typed_list &in, int _piRetCount, typed_list &out)
{
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
        std::string psFieldName = std::string((*itInput)->getAsString()->string_get(0));
        ++itInput;
        pFieldValue = *itInput;
        pRetVal->add(psFieldName, pFieldValue);
    }

    out.push_back(pRetVal);

    return Function::OK;
}
