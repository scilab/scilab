/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Sylvain Genin
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
    extern double C2F(danints)(double*);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_round(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): At least %d expected.\n"), "round", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "round", 1);
        return types::Function::Error;
    }

    if (in[0]->isPoly())
    {
        /***** get data *****/
        types::Polynom* pPolyIn = in[0]->getAs<types::Polynom>();
        int* piRankPolyIn = new int[pPolyIn->getSize()];
        pPolyIn->getRank(piRankPolyIn);

        types::Polynom* pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray(), piRankPolyIn);
        pPolyOut->setComplex(pPolyIn->isComplex());

        /***** perform operation *****/
        for (int compterElem = 0; compterElem < pPolyIn->getSize(); compterElem++)
        {
            types::SinglePoly* pSPIn  = pPolyIn->get(compterElem);
            types::SinglePoly* pSPOut = pPolyOut->get(compterElem);

            double* pRealIn  = pSPIn->get();
            double* pRealOut = pSPOut->get();

            if (pPolyIn->isComplex())
            {
                double* pImgIn  = pSPIn->getImg();
                double* pImgOut = pSPOut->getImg();
                for (int i = 0; i < piRankPolyIn[compterElem] + 1; i++)
                {
                    pRealOut[i] = C2F(danints)(pRealIn + i);
                    pImgOut[i]  = C2F(danints)(pImgIn + i);
                }
            }
            else
            {
                for (int i = 0; i < piRankPolyIn[compterElem] + 1; i++)
                {
                    pRealOut[i] = C2F(danints)(pRealIn + i);
                }
            }
        }

        delete piRankPolyIn;

        /***** return data *****/
        out.push_back(pPolyOut);
        return types::Function::OK;
    }
    else if (in[0]->isDouble())
    {
        /***** get data *****/
        types::Double* pDblIn  = in[0]->getAs<types::Double>();// double
        types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());

        int size = pDblIn->getSize();
        /***** perform operation *****/
        double* pdblInReal  = pDblIn->get();
        double* pDblOutReal = pDblOut->get();
        if (pDblIn->isComplex())
        {
            double* pdblInImg  = pDblIn->getImg();
            double* pDblOutImg = pDblOut->getImg();
            for (int i = 0; i < size; i++)
            {
                pDblOutReal[i] = C2F(danints)(pdblInReal + i);
                pDblOutImg[i]  = C2F(danints)(pdblInImg + i);
            }
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                pDblOutReal[i] = C2F(danints)(pdblInReal + i);
            }
        }

        /***** return data *****/
        out.push_back(pDblOut);
        return types::Function::OK;
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_round";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }
}
/*--------------------------------------------------------------------------*/
