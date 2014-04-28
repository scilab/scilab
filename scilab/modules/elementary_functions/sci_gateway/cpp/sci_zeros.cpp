/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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
#include "api_scilab.hxx"

extern "C"
{
#include "charEncoding.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_zeros(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    api_scilab::Double* pOut = NULL;
    if (in.size() == 0)
    {
        pOut = new api_scilab::Double(0);
    }
    else if (in.size() == 1)
    {
        if (api_scilab::isMatrix(in[0]) == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "zeros", 1);
            return types::Function::Error;
        }

        api_scilab::Matrix* pIn = api_scilab::getAsMatrix(in[0]);
        int iDims = pIn->getDims();
        int* piDims = pIn->getDimsArray();

        // zeros(:)
        if (pIn->getRows() == -1 && pIn->getCols() == -1)
        {
            Scierror(21, _("Invalid index.\n"));
            return types::Function::Error;
        }

        pOut = new api_scilab::Double(iDims, piDims);
        delete pIn;
    }
    else //size > 1
    {
        for (int i = 0 ; i < in.size() ; i++)
        {
            if (api_scilab::isDouble(in[i]) == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "zeros", i + 1);
                return types::Function::Error;
            }
        }

        int iDims = static_cast<int>(in.size());
        int* piDims = new int[iDims];
        for (int i = 0 ; i < iDims ; i++)
        {
            api_scilab::Double* pIn = api_scilab::getAsDouble(in[i]);
            if (pIn->isScalar() == false || pIn->isComplex())
            {
                delete pIn;
                Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "zeros", i + 1);
                return types::Function::Error;
            }
            piDims[i] = static_cast<int>(pIn->getReal()[0]);
            delete pIn;
        }
        pOut = new api_scilab::Double(iDims, piDims);
        delete[] piDims;
    }

    memset(pOut->get(), 0x00, pOut->getSize() * sizeof(double));

    out.push_back(api_scilab::getReturnVariable(pOut));
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
