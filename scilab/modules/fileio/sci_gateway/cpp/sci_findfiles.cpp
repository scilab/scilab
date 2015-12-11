/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
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
#include "double.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "findfiles.h"
#include "localization.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "PATH_MAX.h"
#include "os_string.h"
#include "scicurdir.h"
#include "freeArrayOfString.h"
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define DEFAULT_FILESPEC L"*.*"
#else
#define DEFAULT_FILESPEC L"*"
#endif
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_findfiles(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    wchar_t* pwstPath   = NULL;
    wchar_t* pwstSpec   = NULL;
    bool bAllocatedSpec = false;

    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "findfiles", 0, 2);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        //default path and default file spec
        int ierr        = 0;

        pwstPath        = scigetcwdW(&ierr);
        pwstSpec        = os_wcsdup(DEFAULT_FILESPEC);
        bAllocatedSpec  = true;
    }
    else
    {
        //user path
        if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "findfiles", 1);
            return types::Function::Error;
        }

        pwstPath = expandPathVariableW(in[0]->getAs<types::String>()->get()[0]);

        if (in.size() == 2)
        {
            //user file spec
            if (in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "findfiles", 2);
                return types::Function::Error;
            }

            pwstSpec = in[1]->getAs<types::String>()->get()[0];
        }
        else
        {
            //default file spec
            pwstSpec        = os_wcsdup(DEFAULT_FILESPEC);
            bAllocatedSpec  = true;
        }
    }

    int iSize               = 0;
    wchar_t** pwstFilesList = NULL;

    pwstFilesList = findfilesW(pwstPath, pwstSpec, &iSize, FALSE);
    if (pwstFilesList)
    {
        types::String* pS = new types::String(iSize, 1);
        pS->set(pwstFilesList);
        freeArrayOfWideString(pwstFilesList, iSize);
        out.push_back(pS);
    }
    else
    {
        out.push_back(types::Double::Empty());
    }

    FREE(pwstPath);
    if (bAllocatedSpec)
    {
        FREE(pwstSpec);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
