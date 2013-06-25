/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "core_gw.hxx"
#include "double.hxx"
#include "string.hxx"
#include "function.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
}
/*--------------------------------------------------------------------------*/
#define NB_OF_TYPE 18
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_typename(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iOne = 1;
    wchar_t* pstShortTypeName[NB_OF_TYPE] = {L"s", L"p", L"b", L"sp", L"spb", L"msp", L"i", L"h", L"c",
                                            L"m", L"mc", L"f", L"l", L"tl", L"ml", L"ptr", L"ip", L"fptr"
                                            };
    double pstShortTypeNum[NB_OF_TYPE] = {1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 128, 129, 130};

    /* Check the number of input argument */
    if (in.size() > 2 || in.size() == 1)
    {
        Scierror(77, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "typename" , 0, 2);
        return types::Function::Error;
    }

    /* Check the number of output argument */
    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "typename", 1, 2);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        // get type number
        types::Double* pDblOut = new types::Double(NB_OF_TYPE, 1);
        double* pdblOut = pDblOut->get();
        int iSize = NB_OF_TYPE;
        C2F(dcopy)(&iSize, pstShortTypeNum, &iOne, pdblOut, &iOne);
        out.push_back(pDblOut);

        // get type name
        types::String* pStrOut = new types::String(NB_OF_TYPE, 1);
        for (int i = 0; i < NB_OF_TYPE; i++)
        {
            pStrOut->set(i, pstShortTypeName[i]);
        }
        out.push_back(pStrOut);
    }
    else // in.size() == 2
    {
        Scierror(999, _("%s: Insert a new type is not managed.\n"), "typename");
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
