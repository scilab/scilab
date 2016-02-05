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
#include "double.hxx"
#include "filemanager.hxx"

extern "C"
{
#include <stdio.h>
#include <string.h>
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_merror(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRet  = 0;
    int iFile = -1; //default file : last opened file
    int dimsArray[2] = {1, 1};

    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "merror", 0, 1);
        return types::Function::Error;
    }
    if (in.size() == 1)
    {
        if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "merror", 1);
            return types::Function::Error;
        }

        iFile = (int)in[0]->getAs<types::Double>()->get(0);
    }

    types::File* pF = FileManager::getFile(iFile);
    if (pF != NULL)
    {
        iRet = ferror(pF->getFiledesc());
    }
    else
    {
        if (in.size() == 0)
        {
            iRet = 22;
        }
        else
        {
            Scierror(999, _("%s: Cannot read file whose descriptor is %d: File is not active.\n"), "merror", iFile);
            return types::Function::Error;
        }
    }

    types::Double* pDoubleOut = new types::Double(2, dimsArray);
    pDoubleOut->set(0, iRet);
    out.push_back(pDoubleOut);

    if (_iRetCount == 2)
    {
        types::String* pStringOut = new types::String(2, dimsArray);
        pStringOut->set(0, strerror(iRet));
        out.push_back(pStringOut);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
