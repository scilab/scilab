/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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
#include "sci_malloc.h"
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
