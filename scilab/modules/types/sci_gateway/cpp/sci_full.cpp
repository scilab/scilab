/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bernard HUGUENEY
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

using namespace types;

Function::ReturnValue sci_full(typed_list &in, int nbRes, typed_list &out)
{
    types::Sparse *pRetVal = NULL;

    if(in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"full", 1);
        return Function::Error;
    }

    if(in[0]->isSparse() == false && in[0]->isSparseBool() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: sparse matrix expected.\n"), L"full", 1);
        return Function::Error;
    }

    if(nbRes > 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of output arguments: %d expected.\n"), L"full", 1);
    }

    if(in[0]->isSparse())
    {
        Sparse* pSp = in[0]->getAs<Sparse>();
        Double* pOut = NULL;

        pOut = new Double(pSp->getRows(), pSp->getCols(), pSp->isComplex());
        pSp->fill(*pOut);
        out.push_back(pOut);
    }
    else
    {
        SparseBool* pSb = in[0]->getAs<SparseBool>();
        Bool* pOut = new Bool(pSb->getRows(), pSb->getCols());
        pSb->fill(*pOut);
        out.push_back(pOut);
    }

    //types::Sparse CONST& sp(*(in[0]->getAsSparse()));
    //double* dataReal;
    //double *dataImag;
    //using types::Double;
    //Double* res (sp.isComplex()
    //    ? new Double(sp.rows_get(), sp.cols_get(), &dataReal, &dataImag)
    //    : new Double(sp.rows_get(), sp.cols_get(), &dataReal));
    //if(res)
    //{
    //    sp.fill(*res);
    //    out.push_back(res);
    //}
    return Function::OK;
}
