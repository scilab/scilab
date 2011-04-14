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
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "getrelativefilename.h"
}
/*--------------------------------------------------------------------------*/


Function::ReturnValue sci_getrelativefilename(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int dimsArray[2]    = {1,1};
    wchar_t* wcsAbsDir  = NULL;
    wchar_t* wcsAbsFile = NULL;
    wchar_t* wcsResult  = NULL;

    if(in.size() != 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"getrelativefilename", 2);
        return types::Function::Error;
    }

    if(in[0]->isString() == false || in[0]->getAs<types::String>()->isScalar() == false)
    {
        ScierrorW(999,_W("%ls: Wrong type for input argument #%d: A String expected.\n"), L"getrelativefilename", 1);
        return types::Function::Error;
    }

    if(in[1]->isString() == false || in[1]->getAs<types::String>()->isScalar() == false)
    {
        ScierrorW(999,_W("%ls: Wrong type for input argument #%d: A String expected.\n"), L"getrelativefilename", 2);
        return types::Function::Error;
    }

    wcsAbsDir = expandPathVariableW(in[0]->getAs<types::String>()->get(0));
    if(wcslen(wcsAbsDir) > PATH_MAX)
    {
		ScierrorW(999,_W("%ls: Wrong size for input argument #%d: Must be less than %d characters.\n"), L"getrelativefilename", 1, PATH_MAX);
        return types::Function::Error;
    }
    if(!isdirW(wcsAbsDir))
    {
        ScierrorW(999,_W("%ls: Directory '%ls' doesn't exists.\n"), L"getrelativefilename", wcsAbsDir);
        return types::Function::Error;
    }
    
    wcsAbsFile = expandPathVariableW(in[1]->getAs<types::String>()->get(0));
    if(wcslen(wcsAbsFile) > PATH_MAX)
    {
		ScierrorW(999,_W("%ls: Wrong size for input argument #%d: Must be less than %d characters.\n"), L"getrelativefilename", 2, PATH_MAX);
        return types::Function::Error;
    }
    if(!FileExistW(wcsAbsFile))
    {
        ScierrorW(999,_W("%ls: File '%ls' doesn't exists.\n"), L"getrelativefilename", wcsAbsFile);
        return types::Function::Error;
    }

	wcsResult = getrelativefilenameW(wcsAbsDir, wcsAbsFile);

    FREE(wcsAbsDir);
    FREE(wcsAbsFile);

    types::String* pOut = new types::String(2,dimsArray);
    pOut->set(0, wcsResult);
    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
