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

Function::ReturnValue sci_list(typed_list &in, int _piRetCount, typed_list &out)
{
    List *pRetVal = new List();
    typed_list::iterator itInput;

    for (itInput = in.begin() ; itInput != in.end() ; ++itInput)
    {
        pRetVal->append((*itInput));
    }


    out.push_back(pRetVal);

    return Function::OK;
}
