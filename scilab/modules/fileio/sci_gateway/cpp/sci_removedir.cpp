/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
 *
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include <string.h>
#include "MALLOC.h"
#include "removedir.h"
#include "isdir.h"
#include "expandPathVariable.h"
#include "warningmode.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
}
/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_removedir(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "removedir", 1);
        return Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "removedir", 1);
        return Function::Error;
    }

    wchar_t* pwstPath = expandPathVariableW(in[0]->getAs<types::String>()->get(0));
    int iRet = 0;
    if (!isdirW(pwstPath))
    {
        if (getWarningMode())
        {
            sciprint(_("%s: Warning: Directory '%ls' does not exist.\n"), "removedir", pwstPath);
        }
    }
    else
    {
        iRet = removedirW(pwstPath);
    }

    FREE(pwstPath);
    Bool* pOut = new Bool(iRet);

    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
