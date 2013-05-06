/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
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
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"


extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "sin.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_sin(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn   = NULL;
    types::Double* pDblOut  = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "sin", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "sin", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        pDblIn = in[0]->getAs<types::Double>();
        pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());

        if (pDblIn->isComplex())
        {
            for (int i = 0 ; i < pDblIn->getSize() ; i++)
            {
                zsins(pDblIn->get(i), pDblIn->getImg(i), pDblOut->get() + i, pDblOut->getImg() + i);
            }
        }
        else
        {
            for (int i = 0 ; i < pDblIn->getSize() ; i++)
            {
                pDblOut->set(i, dsins(pDblIn->get(i)));
            }
        }

        out.push_back(pDblOut);
    }
    else if (in[0]->isSparse())
    {
        types::Sparse* pSparseIn = in[0]->getAs<types::Sparse>();
        types::Sparse* pSparseOut = new types::Sparse(pSparseIn->getRows(), pSparseIn->getCols(), pSparseIn->isComplex());

        int const nonZeros = static_cast<int>(pSparseIn->nonZeros());
        int* pRows = new int[nonZeros * 2];
        pSparseIn->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        double* pNonZeroR = new double[nonZeros];
        double* pNonZeroI = new double[nonZeros];
        pSparseIn->outputValues(pNonZeroR, pNonZeroI);

        if (pSparseIn->isComplex())
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                std::complex<double> complex;
                double dblReal = complex.real();
                double dblImg = complex.imag();
                zsins(pNonZeroR[i], pNonZeroI[i], &dblReal, &dblImg);
                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, complex, false);
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, dsins(pNonZeroR[i]), false);
            }
        }

        pSparseOut->finalize();

        delete[] pRows;
        delete[] pNonZeroR;
        delete[] pNonZeroI;

        out.push_back(pSparseOut);
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_sin";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
