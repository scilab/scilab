/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
 *  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "types_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_full(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"full", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of output arguments: %d expected.\n"), L"full", 1);
    }

    if (in[0]->isSparse())
    {
        types::Sparse* pSp = in[0]->getAs<types::Sparse>();
        types::Double* pOut = NULL;

        pOut = new types::Double(pSp->getRows(), pSp->getCols(), pSp->isComplex());
        pSp->fill(*pOut);
        out.push_back(pOut);
    }
    else if (in[0]->isSparseBool())
    {
        types::SparseBool* pSb = in[0]->getAs<types::SparseBool>();
        types::Bool* pOut = new types::Bool(pSb->getRows(), pSb->getCols());
        pSb->fill(*pOut);
        out.push_back(pOut);
    }
    else
    {
        out.push_back(in[0]->clone());
    }

    return types::Function::OK;
}
