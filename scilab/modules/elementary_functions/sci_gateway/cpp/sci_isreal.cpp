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
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_isreal(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double dEps = 0;
    bool bIsComplex = false;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "isreal", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "isreal", 1);
        return types::Function::Error;
    }

    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "isreal", 2);
            return types::Function::Error;
        }

        types::Double* pDblEps = in[1]->getAs<types::Double>();

        if ((pDblEps->isScalar() == false) || pDblEps->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "isreal", 2);
            return types::Function::Error;
        }

        dEps = pDblEps->get(0);
    }

    if (in[0]->isDouble())
    {
        types::Double* pDblIn = in[0]->getAs<types::Double>();

        if (pDblIn->isComplex() == false)
        {
            out.push_back(new types::Bool(true));
            return types::Function::OK;
        }
        else if (pDblIn->isComplex() && in.size() == 1)
        {
            out.push_back(new types::Bool(false));
            return types::Function::OK;
        }

        int size = pDblIn->getSize();
        double* pInI = pDblIn->getImg();
        for (int i = 0; i < size; i++)
        {
            if (fabs(pInI[i]) > dEps)
            {
                out.push_back(new types::Bool(false));
                return types::Function::OK;
            }
        }

        out.push_back(new types::Bool(true));
        return types::Function::OK;
    }
    else if (in[0]->isSparse())
    {
        types::Sparse* pSparseIn = in[0]->getAs<types::Sparse>();

        if (pSparseIn->isComplex() == false)
        {
            out.push_back(new types::Bool(true));
            return types::Function::OK;
        }
        else if (pSparseIn->isComplex() && in.size() == 1)
        {
            out.push_back(new types::Bool(false));
            return types::Function::OK;
        }

        int const nonZeros = static_cast<int>(pSparseIn->nonZeros());
        int* pRows = new int[nonZeros * 2];
        pSparseIn->outputRowCol(pRows);
        int* pCols = pRows + nonZeros;

        for (int i = 0 ; i < nonZeros ; i++)
        {
            std::complex<double> cplxIn = pSparseIn->getImg(pRows[i] - 1, pCols[i] - 1);
            if (fabs(cplxIn.imag()) > dEps)
            {
                delete[] pRows;
                out.push_back(new types::Bool(false));
                return types::Function::OK;
            }
        }

        delete[] pRows;
        out.push_back(new types::Bool(true));
        return types::Function::OK;
    }
    else if (in[0]->isPoly())
    {
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();

        if (pPolyIn->isComplex() == false)
        {
            out.push_back(new types::Bool(true));
            return types::Function::OK;
        }
        else if (pPolyIn->isComplex() && in.size() == 1)
        {
            out.push_back(new types::Bool(false));
            return types::Function::OK;
        }

        for (int i = 0; i < pPolyIn->getSize(); i++)
        {
            int rank = pPolyIn->get(i)->getRank();
            for (int j = 0; j < rank + 1; j++)
            {
                if (fabs(pPolyIn->get(i)->getImg()[j]) > dEps)
                {
                    out.push_back(new types::Bool(false));
                    return types::Function::OK;
                }
            }
        }

        out.push_back(new types::Bool(true));
        return types::Function::OK;
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_isreal";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
