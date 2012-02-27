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
#include "basic_functions.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_clean(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblOut      = NULL;
    types::Polynom* pPolyOut    = NULL;
    types::Sparse* pSparseOut   = NULL;

    double* pdReal  = NULL;
    double* pdImg   = NULL;

    double dNorm    = 0;
    double dEps     = 0;
    double dEpsR    = 1E-10;
    double dEpsA    = 1E-10;

    bool bComplex   = false;
    int iSize       = 0;

    //Only for types::Sparse case
    double* pRows = NULL;
    double* pCols = NULL;

    if (in.size() < 1 || in.size() > 3)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"clean", 1, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"clean", 1);
        return types::Function::Error;
    }

    /***** get data *****/
    if (in[0]->isDouble())
    {
        pDblOut = in[0]->getAs<types::Double>()->clone()->getAs<types::Double>();

        iSize = pDblOut->getSize();
        pdReal = pDblOut->get();
        if (pDblOut->isComplex())
        {
            bComplex = true;
            pdImg = pDblOut->getImg();
        }
    }
    else if (in[0]->isPoly())
    {
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        int* piRanks = new int[pPolyIn->getSize()];
        pPolyIn->getRank(piRanks);
        pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray(), piRanks);

        pDblOut = pPolyIn->getCoef();
        iSize = pDblOut->getSize();
        pdReal = pDblOut->get();
        if (pDblOut->isComplex())
        {
            bComplex = true;
            pdImg = pDblOut->getImg();
        }

        delete[] piRanks;
    }
    else if (in[0]->isSparse())
    {
        types::Sparse* pSparseIn = in[0]->getAs<types::Sparse>();
        pSparseOut = new types::Sparse(pSparseIn->getRows(), pSparseIn->getCols());

        iSize = static_cast<int>(pSparseIn->nonZeros());
        pRows = new double[iSize * 2];
        pSparseIn->outputRowCol(pRows);
        pCols = pRows + iSize;

        pdReal = new double[iSize];
        pdImg  = new double[iSize];
        pSparseIn->outputValues(pdReal, pdImg);

        if (pSparseIn->isComplex())
        {
            bComplex = true;
        }
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_clean";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }


    if (in.size() == 3)
    {
        if (in[2]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real scalar expected.\n"), L"clean", 3);
            return types::Function::Error;
        }

        types::Double* pDbl = in[2]->getAs<types::Double>();

        if (pDbl->isScalar() == false || pDbl->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real scalar expected.\n"), L"clean", 3);
            return types::Function::Error;
        }

        dEpsR = pDbl->get(0);
    }

    if (in.size() >= 2)
    {
        if (in[1]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real scalar expected.\n"), L"clean", 2);
            return types::Function::Error;
        }

        types::Double* pDbl = in[1]->getAs<types::Double>();

        if (pDbl->isScalar() == false || pDbl->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real scalar expected.\n"), L"clean", 2);
            return types::Function::Error;
        }

        dEpsA = pDbl->get(0);
    }

    /***** perform operation *****/
    if (bComplex)
    {
        dNorm = wasums(iSize, pdReal, pdImg);
        dEps = Max(dEpsA, dEpsR * dNorm);

        for (int i = 0 ; i < iSize ; i++)
        {
            if (dabss(pdImg[i]) <= dEps)
            {
                pdImg[i] = 0;
            }
        }
    }
    else
    {
        int iOne = 1;
        dNorm = C2F(dasum)(&iSize, pdReal, &iOne);
        dEps = Max(dEpsA, dEpsR * dNorm);
    }

    for (int i = 0 ; i < iSize ; i++)
    {
        if (dabss(pdReal[i]) <= dEps)
        {
            pdReal[i] = 0;
        }
    }

    /***** set result *****/
    if (in[0]->isDouble())
    {
        out.push_back(pDblOut);
    }
    else if (in[0]->isPoly())
    {
        pPolyOut->setCoef(pDblOut);
        delete pDblOut;
        pDblOut = NULL;
        out.push_back(pPolyOut);
    }
    else if (in[0]->isSparse())
    {
        if (bComplex)
        {
            for (int i = 0 ; i < iSize ; i++)
            {
                std::complex<double> cplx = complex<double>(pdReal[i], pdImg[i]);
                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, cplx);
            }
        }
        else
        {
            for (int i = 0 ; i < iSize ; i++)
            {
                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, pdReal[i]);
            }
        }

        delete[] pdReal;
        delete[] pdImg;
        delete[] pRows;
        out.push_back(pSparseOut);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
