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

extern "C"
{
#include <stdio.h>
#include <string.h>
#include "Scierror.h"
#include "localization.h"
#include "getdrives.h"
#include "freeArrayOfString.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_getdrives(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    wchar_t **wcsDrives = NULL;
    int iNbDrives = 0;
    int dimsArray[2] = {1, 1};
    types::String* pOut = NULL;

    if (in.size() > 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "getdrives", 0);
        return types::Function::Error;
    }

    wcsDrives = getdrivesW(&iNbDrives);
    if (wcsDrives)
    {
        dimsArray[1] = iNbDrives;
        pOut = new types::String(2, dimsArray);
        pOut->set(wcsDrives);
        freeArrayOfWideString(wcsDrives, iNbDrives);
    }
    else
    {
        pOut = new types::String(2, dimsArray);
        pOut->set(0, L"");
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
