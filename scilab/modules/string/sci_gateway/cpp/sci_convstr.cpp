/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#include "string_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_convstr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iConvertMode = -1; // Default is TO_LOWER

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "convstr", 1);
        return types::Function::Error;
    }

    if (in.size() != 1 && in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d or %d expected.\n"), "convstr", 1, 2);
        return types::Function::Error;
    }

    if (in[0]->isString() == false && !(in[0]->isDouble() == true && in[0]->getAs<types::Double>()->isEmpty() == true))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "convstr", 2);
        return types::Function::Error;
    }

    if (in.size() == 2 && in[1]->isString() == true)
    {
        types::String *pInConvertMode = in[1]->getAs<types::String>();
        if (pInConvertMode->getSize() != 1 || wcslen(pInConvertMode->get(0)) != 1)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 'u' (Upper) or 'l' (Lower) expected.\n"), "convstr", 2);
            return types::Function::Error;
        }

        wchar_t wcConvertMode = pInConvertMode->get(0)[0];
        if (wcConvertMode == L'l' || wcConvertMode == L'L' )
        {
            iConvertMode = -1;
        }
        else if (wcConvertMode == L'u' || wcConvertMode == L'U')
        {
            iConvertMode = 1;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 'u' (Upper) or 'l' (Lower) expected.\n"), "convstr", 2);
            return types::Function::Error;
        }
    }

    // Special case convstr([], *) == []
    if (in[0]->isDouble())
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }


    types::String* pstConvertMe = static_cast<types::String*>(in[0]->getAs<types::String>()->clone());

    for (int i = 0 ; i < pstConvertMe->getSize() ; ++i)
    {
        wchar_t *pwcsCurrent = pstConvertMe->get(i);
        for (int j = 0 ; j < wcslen(pwcsCurrent) ; ++j)
        {
            if (iConvertMode == 1)
            {
                pwcsCurrent[j] = towupper(pwcsCurrent[j]);
            }
            else
            {
                pwcsCurrent[j] = towlower(pwcsCurrent[j]);
            }
        }
    }

    out.push_back(pstConvertMe);

    return types::Function::OK;
}
