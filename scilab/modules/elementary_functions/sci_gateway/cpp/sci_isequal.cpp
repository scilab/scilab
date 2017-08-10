/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "bool.hxx"
#include "function.hxx"

types::Function::ReturnValue sci_isequal(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool bResult = true;
    if (in.size() <= 1 || _iRetCount != 1)
    {
        return types::Function::Error;
    }

    for (int i = 1 ; i < static_cast<int>(in.size()); i++)
    {
        types::InternalType* pIn2 = in[i];
        types::InternalType* pIn1 = in[i - 1];

        if (*pIn1 != *pIn2)
        {
            bResult = false;
        }
    }

    types::Bool* pBool = new types::Bool(bResult);

    out.push_back(pBool);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
