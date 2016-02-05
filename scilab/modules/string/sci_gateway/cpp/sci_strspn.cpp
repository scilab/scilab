/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Digiteo 2011 - Cedric DELAMARRE
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


#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "double.hxx"
#include "function.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
}


types::Function::ReturnValue sci_strspn(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pOutDouble   = NULL;
    types::String* pString      = NULL;
    types::String* pStrSample   = NULL;
    int j                       = 0;
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "strspn", 2);
        return types::Function::Error;
    }
    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "strspn", 1);
        return types::Function::Error;
    }
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "strspn", 1);
        return types::Function::Error;
    }
    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "strspn", 2);
        return types::Function::Error;
    }

    pString     = in[0]->getAs<types::String>();
    pStrSample  = in[1]->getAs<types::String>();


    if (pString->getSize() != pStrSample->getSize() && pStrSample->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d.\n"), "strspn", 2);
        return types::Function::Error;
    }

    pOutDouble  = new types::Double(pString->getDims(), pString->getDimsArray());
    for (int i = 0 ; i < pString->getSize() ; i++)
    {
        if (pStrSample->isScalar() == false)
        {
            j = i;
        }

        pOutDouble->set(i, (double)wcsspn( pString->get(i), pStrSample->get(j)));
    }

    out.push_back(pOutDouble);
    return types::Function::OK;
}

