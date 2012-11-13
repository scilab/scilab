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
#include "core_math.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_int(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "int", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "int", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double* pDblIn = in[0]->getAs<types::Double>();
        types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());

        if (pDblIn->isComplex())
        {
            for (int i = 0 ; i < pDblIn->getSize() ; i++)
            {
                if (finite(pDblIn->getImg(i)))
                {
                    pDblOut->setImg(i, (double)(long long int)pDblIn->getImg(i));
                }
                else
                {
                    pDblOut->setImg(i, pDblIn->getImg(i));
                }
            }
        }

        for (int i = 0 ; i < pDblIn->getSize() ; i++)
        {
            if (finite(pDblIn->get(i)))
            {
                pDblOut->set(i, (double)(long long int)pDblIn->get(i));
            }
            else
            {
                pDblOut->set(i, pDblIn->get(i));
            }
        }

        out.push_back(pDblOut);
    }
    else if (in[0]->isSparse())
    {
        types::Sparse* pSparseIn = in[0]->getAs<types::Sparse>();
        types::Sparse* pSparseOut = new types::Sparse(pSparseIn->getRows(), pSparseIn->getCols(), pSparseIn->isComplex());

        int const nonZeros = static_cast<int>(pSparseIn->nonZeros());
        double* pRows = new double[nonZeros * 2];
        pSparseIn->outputRowCol(pRows);
        double* pCols = pRows + nonZeros;

        double* pNonZeroR = new double[nonZeros];
        double* pNonZeroI = new double[nonZeros];
        pSparseIn->outputValues(pNonZeroR, pNonZeroI);

        if (pSparseIn->isComplex())
        {
            for (int i = 0; i < nonZeros; i++)
            {
                if (finite(pNonZeroR[i]))
                {
                    pNonZeroR[i] = (double)(long long int)pNonZeroR[i];
                }

                if (finite(pNonZeroI[i]))
                {
                    pNonZeroI[i] = (double)(long long int)pNonZeroI[i];
                }

                std::complex<double> cplx(pNonZeroR[i], pNonZeroI[i]);
                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, cplx);
            }
        }
        else
        {
            for (int i = 0; i < nonZeros; i++)
            {
                if (finite(pNonZeroR[i]))
                {
                    pNonZeroR[i] = (double)(long long int)pNonZeroR[i];
                }

                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, pNonZeroR[i]);
            }
        }

        delete[] pRows;
        delete[] pNonZeroR;
        delete[] pNonZeroI;

        out.push_back(pSparseOut);
    }
    else if (in[0]->isPoly())
    {
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        types::Polynom* pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray());
        pPolyOut->setComplex(pPolyIn->isComplex());

        double* dataImg  = NULL;
        double* dataReal = NULL;

        if (pPolyIn->isComplex())
        {
            for (int i = 0; i < pPolyIn->getSize(); i++)
            {
                int rank = pPolyIn->get(i)->getRank();
                types::SinglePoly* pSP = new types::SinglePoly(&dataReal, &dataImg, rank);

                for (int j = 0; j < rank; j++)
                {
                    if (finite(pPolyIn->get(i)->getCoefReal()[j]))
                    {
                        dataReal[j] = (double)(long long int)pPolyIn->get(i)->getCoefReal()[j];
                    }
                    else
                    {
                        dataReal[j] = pPolyIn->get(i)->getCoefReal()[j];
                    }

                    if (finite(pPolyIn->get(i)->getCoefImg()[j]))
                    {
                        dataImg[j]  = (double)(long long int)pPolyIn->get(i)->getCoefImg()[j];
                    }
                    else
                    {
                        dataImg[j] = pPolyIn->get(i)->getCoefImg()[j];
                    }
                }

                pPolyOut->set(i, pSP);
                delete pSP;
                pSP = NULL;
            }
        }
        else
        {
            for (int i = 0; i < pPolyIn->getSize(); i++)
            {
                int rank = pPolyIn->get(i)->getRank();
                types::SinglePoly* pSP = new types::SinglePoly(&dataReal, rank);

                for (int j = 0; j < rank; j++)
                {
                    if (finite(pPolyIn->get(i)->getCoefReal()[j]))
                    {
                        dataReal[i] = (double)(long long int)pPolyIn->get(i)->getCoefReal()[j];
                    }
                    else
                    {
                        dataReal[j] = pPolyIn->get(i)->getCoefReal()[j];
                    }
                }

                pPolyOut->set(i, pSP);
                delete pSP;
                pSP = NULL;
            }
        }

        out.push_back(pPolyOut);
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_int";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
