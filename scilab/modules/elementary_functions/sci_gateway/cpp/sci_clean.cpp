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
#include "clean.hxx"
#include "sparse.hxx"
#include "polynom.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_clean(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblOut      = NULL;
    types::Polynom* pPolyOut    = NULL;
    types::Sparse* pSparseOut   = NULL;

    double* pdReal  = NULL;
    double* pdImg   = NULL;

    double dEpsR    = 1E-10;
    double dEpsA    = 1E-10;

    int iSize       = 0;

    //Only for types::Sparse case
    int* pRows = NULL;
    int* pCols = NULL;

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "clean", 1, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "clean", 1);
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
            pdImg = pDblOut->getImg();
        }
    }
    else if (in[0]->isPoly())
    {
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        iSize = pPolyIn->getSize();
        pPolyOut = pPolyIn->clone()->getAs<types::Polynom>();
    }
    else if (in[0]->isSparse())
    {
        types::Sparse* pSparseIn = in[0]->getAs<types::Sparse>();
        pSparseOut = new types::Sparse(pSparseIn->getRows(), pSparseIn->getCols());

        iSize = static_cast<int>(pSparseIn->nonZeros());
        pRows = new int[iSize * 2];
        pSparseIn->outputRowCol(pRows);
        pCols = pRows + iSize;

        pdReal = new double[iSize];
        if (pSparseIn->isComplex())
        {
            pdImg  = new double[iSize];
        }

        pSparseIn->outputValues(pdReal, pdImg);
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_clean";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }


    if (in.size() == 3)
    {
        if (in[2]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "clean", 3);
            if (in[0]->isSparse())
            {
                delete pSparseOut;
                delete[] pdReal;
                delete[] pRows;
                if (pdImg)
                {
                    delete[] pdImg;
                }
            }
            return types::Function::Error;
        }

        types::Double* pDbl = in[2]->getAs<types::Double>();

        if (pDbl->isScalar() == false || pDbl->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "clean", 3);
            if (in[0]->isSparse())
            {
                delete pSparseOut;
                delete[] pdReal;
                delete[] pRows;
                if (pdImg)
                {
                    delete[] pdImg;
                }
            }
            return types::Function::Error;
        }

        dEpsR = pDbl->get(0);
    }

    if (in.size() >= 2)
    {
        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "clean", 2);
            if (in[0]->isSparse())
            {
                delete pSparseOut;
                delete[] pdReal;
                delete[] pRows;
                if (pdImg)
                {
                    delete[] pdImg;
                }
            }
            return types::Function::Error;
        }

        types::Double* pDbl = in[1]->getAs<types::Double>();

        if (pDbl->isScalar() == false || pDbl->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "clean", 2);
            if (in[0]->isSparse())
            {
                delete pSparseOut;
                delete[] pdReal;
                delete[] pRows;
                if (pdImg)
                {
                    delete[] pdImg;
                }
            }
            return types::Function::Error;
        }

        dEpsA = pDbl->get(0);
    }

    /***** perform operation *****/
    if (in[0]->isPoly())
    {
        for (int i = 0 ; i < iSize ; i++)
        {
            types::SinglePoly* pSP = pPolyOut->get(i);
            clean(pSP->get(), pSP->getImg(), pSP->getSize(), dEpsA, dEpsR);
        }
    }
    else
    {
        clean(pdReal, pdImg, iSize, dEpsA, dEpsR);
    }

    /***** set result *****/
    if (in[0]->isDouble())
    {
        out.push_back(pDblOut);
    }
    else if (in[0]->isPoly())
    {
        pPolyOut->updateRank();
        out.push_back(pPolyOut);
    }
    else if (in[0]->isSparse())
    {
        if (pdImg)
        {
            for (int i = 0 ; i < iSize ; i++)
            {
                std::complex<double> cplx = std::complex<double>(pdReal[i], pdImg[i]);
                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, cplx, false);
            }

            delete[] pdImg;
        }
        else
        {
            for (int i = 0 ; i < iSize ; i++)
            {
                pSparseOut->set(pRows[i] - 1, pCols[i] - 1, pdReal[i], false);
            }
        }

        pSparseOut->finalize();

        delete[] pdReal;
        delete[] pRows;
        out.push_back(pSparseOut);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
