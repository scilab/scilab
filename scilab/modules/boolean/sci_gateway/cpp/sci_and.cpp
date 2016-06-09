/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2012 - Allan CORNET
* Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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

#include "boolean_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "overload.hxx"
#include "types.hxx"
#include "bool.hxx"
#include "double.hxx"

extern "C"
{
#include <string.h>
#include "Scierror.h"
#include "localization.h"
#include "vect_and.h"
#include "sci_malloc.h"
}
/*--------------------------------------------------------------------------*/
/* SCILAB function : and */
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_and(types::typed_list &in, int _iRetCount, types::typed_list &out)
{

    int m1 = 0, n1 = 0;
    int opt = 0;
    int *pBoolValuesOne = NULL;
    int *pBoolResult = NULL;

    int *piAddressVarOne = NULL;

    if ((in.size() < 1) || (in.size() > 2))
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "and", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d to %d expected.\n"), "and", 1);
        return types::Function::Error;
    }

    if (in[0]->isGenericType() && in[0]->getAs<types::GenericType>()->getDims() > 2)
    {
        //hypermatrix are manage in external macro
        return Overload::call(L"%hm_and", in, _iRetCount, out);
    }

    if (in[0]->isBool() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_and";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    if (in.size() == 2)
    {
        if (in[1]->getAs<types::GenericType>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "and", 2);
            return types::Function::Error;
        }

        if (in[1]->isString())
        {
            char *pStr =  wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));
            size_t len = strlen(pStr);
            switch (pStr[0])
            {
                case 'r':
                {
                    opt = AND_BY_ROWS;
                    break;
                }
                case '*':
                {
                    opt = AND_ON_ALL;
                    break;
                }
                case 'c':
                {
                    opt = AND_BY_COLUMNS;
                    break;
                }
                default:
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "and", 2);
                    FREE(pStr);
                    return types::Function::Error;
                }
            }

            FREE(pStr);
            if (len != 1)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "and", 2);
                return types::Function::Error;
            }
        }
        else if (in[1]->isDouble())
        {
            types::Double *pdblIn = in[1]->getAs<types::Double>();
            if (pdblIn->isComplex())
            {
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "and", 2);
                return types::Function::Error;
            }

            opt = static_cast<int>(pdblIn->get(0));
            if (opt != pdblIn->get(0))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "and", 2);
                return types::Function::Error;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d.\n"), "and", 2);
            return types::Function::Error;
        }
    }

    types::Bool *pboolIn = in[0]->getAs<types::Bool>();
    int rowIn = pboolIn->getRows();
    int colIn = pboolIn->getCols();
    pBoolValuesOne = pboolIn->get();

    int rowOut = 1;
    int colOut = 1;

    if (opt > 2)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "and", 2);
        return types::Function::Error;
    }

    switch (opt)
    {
        case 1:
            colOut = colIn;
            break;
        case 2 :
            rowOut = rowIn;
            break;
    }

    types::Bool *pboolOut = new types::Bool(rowOut, colOut);
    vect_and(pBoolValuesOne, rowIn, colIn, pboolOut->get(), opt);

    out.push_back(pboolOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
