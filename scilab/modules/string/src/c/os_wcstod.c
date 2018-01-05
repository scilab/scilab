/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2017 - Cedric Delamarre
 *
 * Copyright (C) 2012 - 2017 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
#include <stddef.h>
#include "os_string.h"
#include "os_wcstod.h"
#include "stdio.h"

double os_wcstod(wchar_t *_pwstData, wchar_t** _pwstState)
{
    double val = wcstod(_pwstData, _pwstState);
    wchar_t* pwstStop = *_pwstState;
    if (*pwstStop == L'd' || *pwstStop == L'D')
    {
        wchar_t pwstKey[] = L"1234567890";
        wchar_t pwstSymbol[] = L"-+";
        int iSign = (int)wcscspn(pwstStop, pwstSymbol);
        int iKey = (int)wcscspn(pwstStop, pwstKey);

        if ((iSign == 1 || pwstStop[iSign] == L'\0') && // d-12 or d12
                (iKey  == 1 || iKey == iSign + 1)) // d12 or d+12
        {
            ptrdiff_t iPos = pwstStop - _pwstData;
            wchar_t* pwstData = os_wcsdup(_pwstData);
            pwstData[iPos] = L'e';
            val = wcstod(pwstData, &pwstStop);
            *_pwstState = _pwstData + (pwstStop - pwstData);
            free(pwstData);
        }
    }

    return val;
}
