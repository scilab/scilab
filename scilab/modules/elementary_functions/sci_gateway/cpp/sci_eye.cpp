/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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
#include "context.hxx"

extern "C"
{
#include "basic_functions.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_eye(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRows = 0;
    int iCols = 0;

    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "eye", 0, 2);
        return Function::Error;
    }

    if (in.size() == 0)
    {
        iRows = -1;
        iCols = -1;
    }
    else if (in.size() == 1)
    {
        if (in[0]->isGenericType() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Matrix expected.\n"), "eye", 1);
            return Function::Error;
        }

        iRows = in[0]->getAs<GenericType>()->getRows();
        iCols = in[0]->getAs<GenericType>()->getCols();

        // eye(:)
        if (iRows == -1 && iCols == -1)
        {
            Scierror(21, _("Invalid index.\n"));
            return types::Function::Error;
        }
    }
    else // if (in.size() == 2)
    {
        if (in[0]->isDouble() == false || in[0]->getAs<Double>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "eye", 1);
            return Function::Error;
        }

        if (in[1]->isDouble() == false || in[1]->getAs<Double>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "eye", 2);
            return Function::Error;
        }

        iRows = in[0]->getAs<Double>()->getReal()[0];
        iCols = in[1]->getAs<Double>()->getReal()[0];
    }

    Double* pOut = new Double(iRows, iCols);
    pOut->setZeros();
    for (int i = 0 ; i < Min(iRows, iCols) ; i++)
    {
        pOut->set(i, i, 1);
    }

    if (iRows == -1 && iCols == -1)
    {
        pOut->getReal()[0] = 1;
    }
    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
