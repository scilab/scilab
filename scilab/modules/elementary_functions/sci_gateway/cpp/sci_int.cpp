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
#include "core_math.h"
}
/*
clear a;nb = 2500;a = rand(nb, nb) * 500;tic();int(a);toc
clear a;nb = 2500;a = rand(nb, nb) * 500; a = a + a *%i;tic();int(a);toc
*/
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

        double* pInR = pDblIn->get();
        double* pOutR = pDblOut->get();
        int size = pDblIn->getSize();

        if (pDblIn->isComplex())
        {
            double* pInI = pDblIn->getImg();
            double* pOutI = pDblOut->getImg();
            for (int i = 0; i < size; i++)
            {
                if (finite(pInI[i]))
                {
                    pOutI[i] = (double)(long long int)pInI[i];
                }
                else
                {
                    pOutI[i] = pInI[i];
                }
            }
        }

        for (int i = 0; i < size; i++)
        {
            if (finite(pInR[i]))
            {
                pOutR[i] = (double)(long long int)pInR[i];
            }
            else
            {
                pOutR[i] = pInR[i];
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
                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, cplx, false);
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

                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, pNonZeroR[i], false);
            }
        }

        pSparseOut->finalize();

        delete[] pRows;
        delete[] pNonZeroR;
        delete[] pNonZeroI;

        out.push_back(pSparseOut);
    }
    else if (in[0]->isPoly())
    {
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        types::Polynom* pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray());

        double* dataImg  = NULL;
        double* dataReal = NULL;

        if (pPolyIn->isComplex())
        {
            for (int i = 0; i < pPolyIn->getSize(); i++)
            {
                int rank = pPolyIn->get(i)->getRank();
                types::SinglePoly* pSP = new types::SinglePoly(&dataReal, &dataImg, rank);

                for (int j = 0; j < rank + 1; j++)
                {
                    if (finite(pPolyIn->get(i)->get()[j]))
                    {
                        dataReal[j] = (double)(long long int)pPolyIn->get(i)->get()[j];
                    }
                    else
                    {
                        dataReal[j] = pPolyIn->get(i)->get()[j];
                    }

                    if (finite(pPolyIn->get(i)->getImg()[j]))
                    {
                        dataImg[j]  = (double)(long long int)pPolyIn->get(i)->getImg()[j];
                    }
                    else
                    {
                        dataImg[j] = pPolyIn->get(i)->getImg()[j];
                    }
                }

                pPolyOut->set(i, pSP);
                delete pSP;
                pSP = NULL;
            }
        }
        else
        {
            types::SinglePoly* pSP = NULL;
            for (int i = 0; i < pPolyIn->getSize(); i++)
            {
                int rank = pPolyIn->get(i)->getRank();
                pSP = new types::SinglePoly(&dataReal, rank);

                for (int j = 0; j < rank + 1; j++)
                {
                    double dblVal = pPolyIn->get(i)->get()[j];
                    if (finite(dblVal))
                    {
                        dataReal[j] = (double)((long long)dblVal);
                    }
                    else
                    {
                        dataReal[j] = dblVal;
                    }
                }

                pPolyOut->set(i, pSP);
                delete pSP;
                pSP = NULL;
            }
        }
        out.push_back(pPolyOut);
    }
    else if (in[0]->isInt())
    {
        out.push_back(in[0]);
    }

    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_int";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
