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
types::Function::ReturnValue sci_triu(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iOffset = 0;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "triu", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "triu", 1);
        return types::Function::Error;
    }

    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "triu", 2);
            return types::Function::Error;
        }

        types::Double* pDblOffset = in[1]->getAs<types::Double>();

        if (pDblOffset->isScalar() == false || pDblOffset->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "triu", 2);
            return types::Function::Error;
        }

        iOffset = (int)pDblOffset->get(0);
    }

    /***** get data *****/
    if (in[0]->isDouble()) // double
    {
        types::Double* pDblIn  = in[0]->getAs<types::Double>();
        int iCols = pDblIn->getCols();
        int iRows = pDblIn->getRows();
        double* pdblInReal = pDblIn->get();

        types::Double* pDblOut = new types::Double(iRows, iCols, pDblIn->isComplex());
        double* pdblOutReal = pDblOut->get();
        memset(pdblOutReal, 0x00, iRows * iCols * sizeof(double));

        if (pDblIn->isComplex())
        {
            double* pdblInImg  = pDblIn->getImg();
            double* pdblOutImg = pDblOut->getImg();
            memset(pdblOutImg, 0x00, iRows * iCols * sizeof(double));
            for (int i = 0 ; i < iCols ; i++)
            {
                int iSize = min(max(i + 1 - iOffset, 0), iRows);
                memcpy(&pdblOutReal[i * iRows], &pdblInReal[i * iRows], iSize * sizeof(double));
                memcpy(&pdblOutImg[i * iRows], &pdblInImg[i * iRows], iSize * sizeof(double));
            }
        }
        else
        {
            for (int i = 0 ; i < iCols ; i++)
            {
                int iSize = min(max(i + 1 - iOffset, 0), iRows);
                memcpy(&pdblOutReal[i * iRows], &pdblInReal[i * iRows], iSize * sizeof(double));
            }
        }

        out.push_back(pDblOut);
    }
    else if (in[0]->isPoly()) // polynom
    {
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        int iRows = pPolyIn->getRows();
        int iCols = pPolyIn->getCols();

        types::Polynom* pPolyOut = new types::Polynom(pPolyIn->getVariableName(), iRows, iCols);
        for (int i = 0 ; i < iCols ; i++)
        {
            int iSize = min(max(i + 1 - iOffset, 0), iRows);
            for (int j = 0; j < iSize; j++)
            {
                int iPos = i * iRows + j;
                pPolyOut->set(iPos, pPolyIn->get(iPos));
            }
        }

        out.push_back(pPolyOut);
    }
    else
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_triu";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
