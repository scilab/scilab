/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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
#include "double.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "basic_functions.h"
}

const char fname[] = "sqrt";

types::Function::ReturnValue sci_sqrt(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), fname, 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_sqrt";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    types::Double* input = in[0]->getAs<types::Double>();
    bool isComplex = input->isComplex();
    int size = input->getSize();
    double* pR = input->get();

    //check neg on input values for real matrix
    if (isComplex == false)
    {
        for (int i = 0; i < size; ++i)
        {
            if (pR[i] < 0)
            {
                isComplex = true;
                break;
            }
        }
    }

    types::Double* output = new types::Double(input->getDims(), input->getDimsArray(), isComplex);
    double* pOR = output->get();

    if (isComplex)
    {
        double* pOI = output->getImg();
        if (input->isComplex())
        {
            double* pI = input->getImg();
            for (int i = 0; i < size; ++i)
            {
                zsqrts(pR[i], pI[i], &pOR[i], &pOI[i]);
            }
        }
        else
        {
            for (int i = 0; i < size; ++i)
            {
                zsqrts(pR[i], 0, &pOR[i], &pOI[i]);
            }
        }
    }
    else
    {
        for (int i = 0; i < size; ++i)
        {
            pOR[i] = std::sqrt(pR[i]);
        }
    }

    out.push_back(output);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
