/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "overload.hxx"

extern "C"
{
#include "basic_functions.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_eye(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pOut = NULL;

    int iDims = 0;
    int* piDims = NULL;
    bool alloc = false;

    if (in.size() == 0)
    {
        out.push_back(types::Double::Identity(-1, -1));
        return types::Function::OK;
    }

    bool ret = getDimsFromArguments(in, "eye", &iDims, &piDims, &alloc);
    if (ret == false)
    {
        switch (iDims)
        {
            case -1:
                Scierror(21, _("Invalid index.\n"));
                break;
            case 1:
            {
                //call overload
                return Overload::generateNameAndCall(L"eye", in, _iRetCount, out);
            }
        }

        return types::Function::Error;
    }

    pOut = types::Double::Identity(iDims, piDims);
    if (alloc)
    {
        delete[] piDims;
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
