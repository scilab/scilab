/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
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
#include "string.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_tril(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iOffset = 0;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "tril", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "tril", 1);
        return types::Function::Error;
    }

    // get offset
    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "tril", 2);
            return types::Function::Error;
        }

        types::Double* pDblOffset = in[1]->getAs<types::Double>();

        if (pDblOffset->isScalar() == false || pDblOffset->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "tril", 2);
            return types::Function::Error;
        }

        iOffset = (int)pDblOffset->get(0);
    }

    // perform operation
    if (in[0]->isDouble()) // double
    {
        types::Double* pDblOut = in[0]->getAs<types::Double>()->clone()->getAs<types::Double>();
        int iRows = pDblOut->getRows();
        int iCols = pDblOut->getCols();
        double* pdblOutReal = pDblOut->get();

        if (pDblOut->isComplex())
        {
            double* pdblOutImg = pDblOut->getImg();
            for (int i = 0; i < iCols; i++)
            {
                int iSize = min(max(i - iOffset, 0), iRows);
                memset(&pdblOutReal[i * iRows], 0x00, iSize * sizeof(double));
                memset(&pdblOutImg[i * iRows],  0x00, iSize * sizeof(double));
            }
        }
        else
        {
            for (int i = 0; i < iCols; i++)
            {
                int iSize = min(max(i - iOffset, 0), iRows);
                memset(&pdblOutReal[i * iRows], 0x00, iSize * sizeof(double));
            }
        }

        out.push_back(pDblOut);
    }
    else if (in[0]->isPoly()) // polynom
    {
        types::Polynom* pPolyOut = in[0]->getAs<types::Polynom>()->clone()->getAs<types::Polynom>();
        int iRows = pPolyOut->getRows();
        int iCols = pPolyOut->getCols();

        if (pPolyOut->isComplex())
        {
            for (int i = 0; i < iCols; i++)
            {
                int iSize = min(max(i - iOffset, 0), iRows);
                for (int j = 0; j < iSize; j++)
                {
                    double* dblReal = NULL;
                    double* dblImg  = NULL;
                    types::SinglePoly* pSP = new types::SinglePoly(&dblReal, &dblImg, 1);
                    dblReal[0] = 0;
                    dblImg[0]  = 0;
                    pPolyOut->set(i * iRows + j, pSP);
                    delete pSP;
                }
            }
        }
        else
        {
            for (int i = 0; i < iCols; i++)
            {
                int iSize = min(max(i - iOffset, 0), iRows);
                for (int j = 0; j < iSize; j++)
                {
                    double* dbl = NULL;
                    types::SinglePoly* pSP = new types::SinglePoly(&dbl, 1);
                    dbl[0] = 0;
                    pPolyOut->set(i * iRows + j, pSP);
                    delete pSP;
                }
            }
        }

        out.push_back(pPolyOut);
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_tril";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
