/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_sciargs(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iCount = 0;
    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "sciargs", 0);
        return types::Function::Error;
    }

    wchar_t** pwstCmdLineArgs = ConfigVariable::getCommandLineArgs(&iCount);

    if (iCount == 0)
    {
        // call_scilab
        out.push_back(new types::String(L""));
        return types::Function::OK;
    }

    types::String* pS = new types::String(iCount, 1);
    pS->set(pwstCmdLineArgs);
    out.push_back(pS);
    freeArrayOfWideString(pwstCmdLineArgs, iCount);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
