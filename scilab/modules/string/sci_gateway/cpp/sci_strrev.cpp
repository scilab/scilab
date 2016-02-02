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
#include "funcmanager.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "freeArrayOfString.h"
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
#include "stringsstrrev.h"
}

types::Function::ReturnValue sci_strrev(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pString      = NULL;
    types::String* pOutString   = NULL;
    wchar_t **OutputStrings     = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "strrev", 1);
        return types::Function::Error;
    }
    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "strrev", 1);
        return types::Function::Error;
    }
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of Strings expected.\n"), "strrev", 1);
        return types::Function::Error;
    }

    pString = in[0]->getAs<types::String>();
    // wchar_t* pwstStr = in[0]->getAs<types::String>()->get(0);

    OutputStrings = strings_strrev(pString->get(), pString->getSize());

    pOutString = new types::String(pString->getDims(), pString->getDimsArray());
    pOutString->set(OutputStrings);
    freeArrayOfWideString(OutputStrings, pString->getSize());
    out.push_back(pOutString);
    return types::Function::OK;
}

