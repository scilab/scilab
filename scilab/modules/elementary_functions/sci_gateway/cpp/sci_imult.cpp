/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
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

#include <complex>

#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "sparse.hxx"
#include "polynom.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
}

/*
clear a;nb = 2500;a = rand(nb, nb);tic();imult(a);toc
clear a;nb = 2500;a = rand(nb, nb); a = a + a *%i;tic();imult(a);toc
*/
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_imult(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "imult", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "imult", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double* pDblIn = in[0]->getAs<types::Double>();
        types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), true);

        int iSize = pDblIn->getSize();
        int iOne = 1;

        if (pDblIn->isComplex())
        {
            double* pOutR = pDblOut->get();
            double* pInI = pDblIn->getImg();
            for (int i = 0; i < iSize; i++)
            {
                pOutR[i] = pInI[i] * -1;
            }
        }
        else
        {
            memset(pDblOut->getReal(), 0x00, iSize * sizeof(double));
        }

        C2F(dcopy)(&iSize, pDblIn->getReal(), &iOne, pDblOut->getImg(), &iOne);

        out.push_back(pDblOut);
    }
    else if (in[0]->isSparse())
    {
        types::Sparse* pSparseIn = in[0]->getAs<types::Sparse>();
        types::Sparse* pSparseOut = new types::Sparse(pSparseIn->getRows(), pSparseIn->getCols(), true);

        int const nonZeros = static_cast<int>(pSparseIn->nonZeros());
        int* pRows = new int[nonZeros * 2];
        pSparseIn->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        if (pSparseIn->isComplex())
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                std::complex<double> cplxIn = pSparseIn->getImg(pRows[i] - 1, pCols[i] - 1);
                std::complex<double> cplxOut(cplxIn.imag() * -1, cplxIn.real());
                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, cplxOut);
            }
        }
        else
        {
            for (int i = 0 ; i < nonZeros ; i++)
            {
                double dReal = pSparseIn->get(pRows[i] - 1, pCols[i] - 1);
                std::complex<double> cplxOut(0, dReal);
                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, cplxOut);
            }
        }

        delete[] pRows;

        out.push_back(pSparseOut);
    }
    else if (in[0]->isPoly())
    {
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        types::Polynom* pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray());
        pPolyOut->setComplex(true);

        double* dataImg  = NULL;
        double* dataReal = NULL;

        for (int i = 0; i < pPolyIn->getSize(); i++)
        {
            int rank = pPolyIn->get(i)->getRank();
            types::SinglePoly* pSP = new types::SinglePoly(&dataReal, &dataImg, rank);

            if (pPolyIn->isComplex())
            {
                for (int j = 0; j < rank + 1; j++)
                {
                    dataReal[j] = pPolyIn->get(i)->getImg()[j] * -1;
                    dataImg[j]  = pPolyIn->get(i)->get()[j];
                }
            }
            else
            {
                for (int j = 0; j < rank + 1; j++)
                {
                    dataReal[j] = 0;
                    dataImg[j]  = pPolyIn->get(i)->get()[j];
                }
            }

            pPolyOut->set(i, pSP);
            delete pSP;
            pSP = NULL;
        }

        out.push_back(pPolyOut);
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_imult";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
