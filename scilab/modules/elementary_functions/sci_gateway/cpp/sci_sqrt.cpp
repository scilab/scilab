/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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
