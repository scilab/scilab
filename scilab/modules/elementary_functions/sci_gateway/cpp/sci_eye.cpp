/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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
#include "overload.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include "basic_functions.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}


int mint(int *tab, int length)
{
    int ret  = *tab;
    for (int i = 1; i < length ; i++)
    {
        if (ret > tab[i])
        {
            ret = tab[i];
        }
    }
    return ret;
}

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_eye(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRows = 0;
    int iCols = 0;
    int dims = 2;
    int *dimsArray = new int[in.size()];
    int *retArray = NULL;

    if (in.size() == 0)
    {
        dimsArray[0] = -1;
        dimsArray[1] = -1;

    }
    else if (in.size() == 1)
    {
        if (in[0]->isGenericType() == false)
        {
            std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_eye";
            return Overload::call(wstFuncName, in, _iRetCount, out, new ast::ExecVisitor());
        }

        dimsArray[0] = in[0]->getAs<types::GenericType>()->getRows();
        dimsArray[1] = in[0]->getAs<types::GenericType>()->getCols();
        // eye(:)
        if (dimsArray[1] == -1 && dimsArray[0] == -1)
        {
            Scierror(21, _("Invalid index.\n"));
            return types::Function::Error;
        }
    }
    else // if (in.size() >= 2)
    {
        dims = (int)in.size();
        for (int i = 0; i < (int)in.size(); i++)
        {
            if (in[i]->isDouble() == false || in[i]->getAs<types::Double>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Real scalar expected.\n"), "eye", i + 1);
                return Function::Error;
            }
            dimsArray[i] = (int)in[i]->getAs<types::Double>()->get()[0];
        }
    }

    Double* pOut = new Double(dims, dimsArray);
    int* piCoords = new int[pOut->getDims()];
    pOut->setZeros();
    int iMinDims = mint(dimsArray, dims);
    for (int i = 0; i < iMinDims; i++)
    {
        for (int j = 0 ; j < dims ; j++)
        {
            piCoords[j] = i;
        }

        pOut->set(pOut->getIndex(piCoords), 1);
    }

    if (iMinDims == -1)
    {
        pOut->set(0, 1);
    }

    delete[] piCoords;
    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
