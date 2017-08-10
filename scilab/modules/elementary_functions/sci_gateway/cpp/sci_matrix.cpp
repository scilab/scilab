/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
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
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "overload.hxx"

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
        Scierror(77, _("%s: Wrong number of input argument(s): At least %d expected.\n"), "matrix", 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "matrix", 1);
        return types::Function::Error;
    }

    if (in[0]->isArrayOf()      == false &&
            in[0]->isSparse()       == false &&
            in[0]->isSparseBool()   == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_matrix";
        return Overload::call(wstFuncName, in, _iRetCount, out);
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
            Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), "matrix", 2);
            pGTOut->killMe();
            return types::Function::Error;
        }

        types::Double* pDblNewSize = in[1]->getAs<types::Double>();

        if (pDblNewSize->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : A real matrix expected.\n"), "matrix", 2);
            pGTOut->killMe();
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
                    Scierror(999, _("%s: Wrong value for input argument #%d : Only one value can be equal to %d.\n"), "matrix", 2, -1);
                    pGTOut->killMe();
                    delete[] piSizes;
                    return types::Function::Error;
                }
            }
            else if (piSizes[i] < -1)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d : At most %d expected.\n"), "matrix", 2, -1);
                pGTOut->killMe();
                delete[] piSizes;
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
        iDims = static_cast<int>(in.size()) - 1;
        piSizes = new int[iDims];
        for (int i = 1; i < static_cast<int>(in.size()); i++)
        {
            if (in[i]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "matrix", i + 1);
                pGTOut->killMe();
                delete[] piSizes;
                return types::Function::Error;
            }

            types::Double* pDblNewSize = in[i]->getAs<types::Double>();

            if (pDblNewSize->isComplex() || pDblNewSize->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d : A real scalar expected.\n"), "matrix", i + 1);
                pGTOut->killMe();
                delete[] piSizes;
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
                    Scierror(999, _("%s: Wrong value for input argument #%d : Only one value can be equal to %d.\n"), "matrix", i + 1, -1);
                    pGTOut->killMe();
                    delete[] piSizes;
                    return types::Function::Error;
                }
            }
            else if (piSizes[i - 1] < -1)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d : At most %d expected.\n"), "matrix", i + 1, -1);
                pGTOut->killMe();
                delete[] piSizes;
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
        Scierror(999, _("%s: Wrong value for input argument(s) : Sparse matrix cannot be reshaped beyond %d dimensions.\n"), "matrix", 2);
        pGTOut->killMe();
        delete[] piSizes;
        return types::Function::Error;
    }

    bOk = pGTOut->reshape(piSizes, iDims);
    delete[] piSizes;

    if (bOk == false)
    {
        Scierror(999, _("%s: Input and output matrices must have the same number of elements.\n"), "matrix");
        pGTOut->killMe();
        return types::Function::Error;
    }

    out.push_back(pGTOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
