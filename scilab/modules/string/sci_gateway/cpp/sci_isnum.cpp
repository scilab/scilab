/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
 * Copyright (C) 2011 - INRIA - Michael Baudin
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * This code is also published under the GPL v3 license.
 *
 */

#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "bool.hxx"
#include "double.hxx"
#include "function.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
#include "isnum.h"
#include "BOOL.h"
}


types::Function::ReturnValue sci_isnum(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Bool* pOutBool   = NULL;
    types::String* pString  = NULL;
    BOOL *values            = NULL;

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

    values = isNumMatrixW(const_cast<const wchar_t**>(pString->get()), pString->getRows(), pString->getCols());

    pOutBool = new types::Bool(pString->getRows(), pString->getCols());
    pOutBool->set((int*)values);
    out.push_back(pOutBool);
    return types::Function::OK;
}
// =============================================================================
