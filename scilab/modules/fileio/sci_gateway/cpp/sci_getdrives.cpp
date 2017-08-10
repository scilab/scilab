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
