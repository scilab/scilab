/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "function.hxx"
#include "double.hxx"
#include "list.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

Function::ReturnValue sci_getfield(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 2)
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"getfield", 2);
        return Function::Error;
    }

    if (in[0]->isDouble() == false || in[0]->getAs<Double>()->isComplex() == true)
    {
        ScierrorW(999, _W("%ls:  Wrong type for input argument #%d: Real Matrix expected.\n"), L"getfield", 1);
        return Function::Error;
    }

    if (in[1]->isList() == false && in[1]->isMList() == false && in[1]->isTList() == false)
    {
        ScierrorW(999, _W("%ls:  Wrong type for input argument #%d: List expected.\n"), L"getfield", 2);
        return Function::Error;
    }

    // FIXME : Scilab < 6 allows mlist wrapped types like struct, hypermat, aso...
    // Need to investigate if this feature is mandatory.

    Double *pInDouble = in[0]->getAs<Double>();

    if (_iRetCount != pInDouble->getSize())
    {
        ScierrorW(999, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"getfield", pInDouble->getSize());
        return Function::Error;
    }

    List *pInList = in[1]->getAs<List>();
    double *pdblReal = pInDouble->get();

    for (int i = 0 ; i < pInDouble->getSize() ; ++i)
    {
        const int iIndex = ((int) pdblReal[i]) - 1;
        InternalType *pIT = pInList->get(iIndex);
        if (pIT != NULL)
        {
            out.push_back(pIT);
        }
        else
        {
            ScierrorW(21, _W("%ls: Invalid index: Computed index was %d.\n"), L"getfield", iIndex + 1);
            return Function::Error;
        }
    }

    return Function::OK;
}
