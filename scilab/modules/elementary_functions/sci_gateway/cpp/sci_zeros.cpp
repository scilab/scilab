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
#include "function.hxx"
#include "double.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_zeros(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    Double* pOut = NULL;
    if (in.size() == 0)
    {
        pOut = new Double(0);
    }
    else if (in.size() == 1)
    {
        if (in[0]->isGenericType() == false || in[0]->isContainer())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "zeros", 1);
            return Function::Error;
        }

        Double* pIn = in[0]->getAs<Double>();
        int iDims = pIn->getDims();
        int* piDims = pIn->getDimsArray();

        // zeros(:)
        if (pIn->getRows() == -1 && pIn->getCols() == -1)
        {
            Scierror(21, _("Invalid index.\n"));
            return Function::Error;
        }

        pOut = new Double(iDims, piDims);
    }
    else //size > 1
    {
        for (int i = 0 ; i < in.size() ; i++)
        {
            if (in[i]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "zeros", i + 1);
                return Function::Error;
            }
        }

        int iDims = static_cast<int>(in.size());
        int* piDims = new int[iDims];
        for (int i = 0 ; i < iDims ; i++)
        {
            Double* pIn = in[i]->getAs<Double>();
            if (pIn->isScalar() == false || pIn->isComplex())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "zeros", i + 1);
                return Function::Error;
            }
            piDims[i] = static_cast<int>(pIn->getReal()[0]);
        }
        pOut = new Double(iDims, piDims);
        delete[] piDims;
    }

    pOut->setZeros();

    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
