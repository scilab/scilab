/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
 * Copyright (C) 2011 - INRIA - Michael Baudin
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
 * This code is also published under the GPL v3 license.
 *
 */

#include "function.hxx"
#include "string.hxx"
#include "bool.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "isnum.h"
#include "BOOL.h"
}


types::Function::ReturnValue sci_isnum(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Bool* pOutBool   = NULL;
    types::String* pString  = NULL;
    int *values             = NULL;
    wchar_t **strVal        = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "isnum", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "isnum", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "isnum", 1);
        return types::Function::Error;
    }

    pString = in[0]->getAs<types::String>();
    pOutBool = new types::Bool(pString->getDims(), pString->getDimsArray());
    values =  pOutBool->get();
    strVal = pString->get();
    for (int i = 0; i < pString->getSize(); i++)
    {
        values[i] = (int)isNumW(strVal[i]);
    }
    out.push_back(pOutBool);
    return types::Function::OK;
}
// =============================================================================
