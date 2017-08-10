/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
 *
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
#include "funcmanager.hxx"
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "filemanager.hxx"

extern "C"
{
#include <stdio.h>
#include <string.h>
#include "PATH_MAX.h"
#include "isdir.h"
#include "FileExist.h"
#include "expandPathVariable.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "getrelativefilename.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_getrelativefilename(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    wchar_t* wcsAbsDir  = NULL;
    wchar_t* wcsAbsFile = NULL;
    wchar_t* wcsResult  = NULL;

    types::String* pStrDir  = NULL;
    types::String* pStrFile = NULL;
    types::String* pStrOut  = NULL;

    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "getrelativefilename", 2);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of strings expected.\n"), "getrelativefilename", 1);
        return types::Function::Error;
    }

    pStrDir = in[0]->getAs<types::String>();

    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of strings expected.\n"), "getrelativefilename", 2);
        return types::Function::Error;
    }

    pStrFile = in[1]->getAs<types::String>();

    if (pStrDir->getSize() != pStrFile->getSize())
    {
        Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same size expected.\n"), "getrelativefilename", 1, 2);
        return types::Function::Error;
    }

    pStrOut = new types::String(pStrDir->getDims(), pStrDir->getDimsArray());

    for (int i = 0; i < pStrDir->getSize(); i++)
    {
        wcsAbsDir = expandPathVariableW(pStrDir->get(i));
        if (wcslen(wcsAbsDir) > PATH_MAX)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Must be less than %d characters.\n"), "getrelativefilename", 1, PATH_MAX);
            FREE(wcsAbsDir);
            delete pStrOut;
            return types::Function::Error;
        }

        wcsAbsFile = expandPathVariableW(pStrFile->get(i));
        if (wcslen(wcsAbsFile) > PATH_MAX)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Must be less than %d characters.\n"), "getrelativefilename", 2, PATH_MAX);
            FREE(wcsAbsFile);
            FREE(wcsAbsDir);
            delete pStrOut;
            return types::Function::Error;
        }

        wcsResult = getrelativefilenameW(wcsAbsDir, wcsAbsFile);

        FREE(wcsAbsDir);
        FREE(wcsAbsFile);

        pStrOut->set(i, wcsResult);
        FREE(wcsResult);
    }

    out.push_back(pStrOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
