/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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

#include "sparse_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "sparse.hxx"
#include "pointer.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
#include "lu.h"
}

types::Function::ReturnValue sci_lufact(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Sparse *pSpIn = NULL;

    double abstol   = 0;
    double reltol   = 0.001;
    int nrank       = 0;
    int ierr        = 0;
    int m           = 0;
    int n           = 0;
    int nonZeros    = 0;

    double* dbl     = NULL;
    int* colPos     = NULL;
    int* itemsRow   = NULL;
    int* fmatindex  = NULL;

    //check input parameters
    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "lufact", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): At most %d expected.\n"), "lufact", 2);
        return types::Function::Error;
    }

    abstol = nc_eps_machine();

    //call format
    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false)
        {
            Scierror(77, _("%s: Wrong type for argument %d: Matrix expected.\n"), "lufact", 2);
            return types::Function::Error;
        }

        types::Double *pDblPrec = in[1]->getAs<types::Double>();

        if (pDblPrec->getSize() > 2)
        {
            Scierror(77, _("%s: Wrong number of dimensions for input argument #%d.\n"), "lufact", 2);
            return types::Function::Error;
        }

        if (pDblPrec->getSize() == 2)
        {
            reltol = pDblPrec->get(1);
        }

        abstol = pDblPrec->get(0);
    }

    if (in[0]->isSparse() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_lufact";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pSpIn = in[0]->getAs<types::Sparse>();

    m = pSpIn->getRows();
    n = pSpIn->getCols();

    if (m != n)
    {
        Scierror(77, _("%s: Wrong size for input argument #%d: Square matrix expected.\n"), "lufact", 1);
        return types::Function::Error;
    }

    if (pSpIn->isComplex())
    {
        Scierror(77, _("%s: Wrong type for argument %d: Real matrix expected.\n"), "lufact", 1);
        return types::Function::Error;
    }

    nonZeros = (int)pSpIn->nonZeros();
    dbl = new double[nonZeros];
    colPos = new int[nonZeros];
    itemsRow = new int[m];
    fmatindex = new int[1];

    pSpIn->outputValues(dbl, NULL);
    pSpIn->getNbItemByRow(itemsRow);
    pSpIn->getColPos(colPos);

    C2F(lufact1)(dbl, itemsRow, colPos, &m, &nonZeros, fmatindex, &abstol, &reltol, &nrank, &ierr);

    out.push_back(new types::Pointer(m, n, (void*)fmatindex, pSpIn->isComplex()));

    if (_iRetCount == 2)
    {
        out.push_back(new types::Double((double)nrank));
    }

    delete[] dbl;
    delete[] colPos;
    delete[] itemsRow;

    return types::Function::OK;
}
