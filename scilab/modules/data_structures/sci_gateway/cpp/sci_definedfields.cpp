/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "data_structures_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "list.hxx"
#include "mlist.hxx"
#include "tlist.hxx"
#include "struct.hxx"

extern "C"
{
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
#include "freeArrayOfString.h"
}

/*-----------------------------------------------------------------------------------*/
types::Function::ReturnValue sci_definedfields(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "definedfields", 1);
        return types::Function::Error;
    }

    types::List* pL = in[0]->getAs<types::List>();
    if (pL->isList() == false && pL->isMList() == false && pL->isTList() == false)
    {
        Scierror(999, _("%s:  Wrong type for input argument #%d: List expected.\n"), "getfield", 2);
        return types::Function::Error;
    }

    double* res = new double[pL->getSize()];
    int idx = 0;
    for (int i = 0 ; i < pL->getSize() ; i++)
    {
        types::InternalType* pIT = pL->get(i);
        if (pIT->getType() != types::InternalType::ScilabListUndefinedOperation)
        {
            res[idx++] = i + 1;
        }
    }

    types::Double* pDbl = new types::Double(1, idx);
    pDbl->set(res);
    out.push_back(pDbl);
    delete[] res;
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
