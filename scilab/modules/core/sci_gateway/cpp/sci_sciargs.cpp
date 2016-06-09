/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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
        freeArrayOfWideString(pwstCmdLineArgs, iCount);
        return types::Function::OK;
    }

    types::String* pS = new types::String(iCount, 1);
    pS->set(pwstCmdLineArgs);
    out.push_back(pS);
    freeArrayOfWideString(pwstCmdLineArgs, iCount);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
