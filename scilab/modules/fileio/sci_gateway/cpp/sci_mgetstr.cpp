/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "charEncoding.h"
#include "mgetstr.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_mgetstr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iDims                   = 2;
    int iDimsArray[2]           = {1, 1};
    int iFile                   = -1; //default file : last opened file
    types::String* pOutString   = NULL;
    int iSizeToRead             = 0;
    wchar_t* pwstOut            = NULL;


    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "mgetstr", 1, 2);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "mgetstr", 1);
        return types::Function::Error;
    }

    iSizeToRead = static_cast<int>(in[0]->getAs<types::Double>()->get(0));

    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false || in[1]->getAs<types::Double>()->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "mgetstr", 2);
            return types::Function::Error;
        }
        iFile = static_cast<int>(in[1]->getAs<types::Double>()->get(0));
    }
    switch (iFile)
    {
        case 0: // stderr
        case 6: // stdout
            Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mgetstr", iFile);
            return types::Function::Error;
        default :
            pwstOut = mgetstr(iFile, iSizeToRead);
    }

    if (pwstOut == NULL)
    {
        Scierror(999, _("%s: Unable to read file %d.\n"), "mgetstr", iFile);
        return types::Function::Error;
    }

    out.push_back(new types::String(pwstOut));
    FREE(pwstOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
