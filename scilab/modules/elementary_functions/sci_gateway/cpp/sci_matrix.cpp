/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
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
types::Function::ReturnValue sci_matrix(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::GenericType* pGTIn  = NULL;
    types::GenericType* pGTOut = NULL;
    int* piSizes    = NULL;
    int iDims       = 0;
    int iLeastOne   = -1;
    int newSize     = 1;
    bool bOk        = false;

    if (in.size() < 2 )
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): At least %d expected.\n"), L"matrix", 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"matrix", 1);
        return types::Function::Error;
    }

    if (in[0]->isGenericType() == false)
    {
        std::wstring wstFuncName = L"%"  + in[0]->getShortTypeStr() + L"_matrix";
        return Overload::call(wstFuncName, in, _iRetCount, out, new ExecVisitor());
    }

    pGTIn = in[0]->getAs<types::GenericType>();
    if (pGTIn->getSize() == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    pGTOut = pGTIn->clone()->getAs<types::GenericType>();

    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real matrix expected.\n"), L"matrix", 2);
            return types::Function::Error;
        }

        types::Double* pDblNewSize = in[1]->getAs<types::Double>();

        if (pDblNewSize->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real matrix expected.\n"), L"matrix", 2);
            return types::Function::Error;
        }

        iDims = pDblNewSize->getSize();
        piSizes = new int[iDims];

        for (int i = 0; i < iDims; i++)
        {
            piSizes[i] = static_cast<int>(pDblNewSize->get(i));
            if (piSizes[i] == -1)
            {
                if (iLeastOne == -1)
                {
                    iLeastOne = i;
                }
                else
                {
                    ScierrorW(999, _W("%ls: Wrong value for input argument #%d : Only one value can be equal to %d.\n"), L"matrix", 2, -1);
                    return types::Function::Error;
                }
            }
            else if (piSizes[i] < -1)
            {
                ScierrorW(999, _W("%ls: Wrong value for input argument #%d : At most %d expected.\n"), L"matrix", 2, -1);
                return types::Function::Error;
            }
            else
            {
                newSize *= piSizes[i];
            }
        }
    }
    else
    {
        iDims = in.size() - 1;
        piSizes = new int[iDims];
        for (int i = 1; i < in.size(); i++)
        {
            if (in[i]->isDouble() == false)
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real scalar expected.\n"), L"matrix", i + 1);
                return types::Function::Error;
            }

            types::Double* pDblNewSize = in[i]->getAs<types::Double>();

            if (pDblNewSize->isComplex() || pDblNewSize->isScalar() == false)
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A real scalar expected.\n"), L"matrix", i + 1);
                return types::Function::Error;
            }

            piSizes[i - 1] = static_cast<int>(pDblNewSize->get(0));
            if (piSizes[i - 1] == -1)
            {
                if (iLeastOne == -1)
                {
                    iLeastOne = i - 1;
                }
                else
                {
                    ScierrorW(999, _W("%ls: Wrong value for input argument #%d : Only one value can be equal to %d.\n"), L"matrix", i + 1, -1);
                    return types::Function::Error;
                }
            }
            else if (piSizes[i - 1] < -1)
            {
                ScierrorW(999, _W("%ls: Wrong value for input argument #%d : At most %d expected.\n"), L"matrix", i + 1, -1);
                return types::Function::Error;
            }
            else
            {
                newSize *= piSizes[i - 1];
            }
        }
    }

    if (iLeastOne != -1)
    {
        piSizes[iLeastOne] = (int)pGTOut->getSize() / newSize;
    }

    if (pGTOut->isSparse() && iDims > 2)
    {
        ScierrorW(999, _W("%ls: Wrong value for input argument(s) : Sparse matrix cannot be reshaped beyond %d dimensions.\n"), L"matrix", 2);
        return types::Function::Error;
    }

    bOk = pGTOut->reshape(piSizes, iDims);

    if (bOk == false)
    {
        ScierrorW(999, _W("%ls: Input and output matrices must have the same number of elements.\n"), L"matrix");
        return types::Function::Error;
    }

    delete[] piSizes;

    out.push_back(pGTOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
