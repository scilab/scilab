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

#include "sparse_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

//[xadj,adjncy,anz]= sp2adj(
types::Function::ReturnValue sci_sp2adj(types::typed_list &in, int nbRes, types::typed_list &out)
{
    types::Sparse *pRetVal = NULL;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "sp2adj", 1);
        return types::Function::Error;
    }

    if (in[0]->isSparse() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: sparse matrix expected.\n"), "sp2adj", 1);
        return types::Function::Error;
    }
    if (nbRes > 3)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d to %d expected.\n"), "sp2adj", 1, 3);
        return types::Function::Error;
    }

    types::InternalType* pIT = NULL;
    types::Sparse* SPARSE_CONST spIn = in[0]->getAs<types::Sparse>();
    spIn->transpose(pIT);
    types::Sparse* sp = pIT->getAs<types::Sparse>();
    std::size_t const nonZeros = sp->nonZeros();

    types::Double* res = new types::Double(spIn->getCols() + 1, 1);
    res->set(0, 1);
    for (std::size_t i = 0; i != spIn->getCols() ; i++)
    {
        res->set(static_cast<int>(i + 1), res->get(static_cast<int>(i)) + sp->nonZeros(i));
    }

    out.push_back(res);

    if (nbRes >= 2)
    {
        res = new types::Double(static_cast<int>(nonZeros), 1);
        sp->outputCols(res->getReal());
        for (int i = 0 ; i < res->getSize() ; i++)
        {
            res->getReal()[i]++;
        }
        out.push_back(res);
    }

    if (nbRes == 3)
    {
        res = new types::Double(static_cast<int>(nonZeros), 1, sp->isComplex());
        sp->outputValues(res->getReal(), res->getImg());
        out.push_back(res);
    }

    delete pIT;
    return types::Function::OK;
}
