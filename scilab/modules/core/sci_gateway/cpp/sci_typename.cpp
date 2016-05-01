/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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
    const wchar_t* pstShortTypeName[NB_OF_TYPE] = {L"s", L"p", L"b", L"sp", L"spb", L"msp", L"i", L"h", L"c",
                                                   L"m", L"mc", L"f", L"l", L"tl", L"ml", L"ptr", L"ip", L"fptr"
                                                  };
    double pstShortTypeNum[NB_OF_TYPE] = {1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 128, 129, 130};

    /* Check the number of input argument */
    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "typename" , 0);
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

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
