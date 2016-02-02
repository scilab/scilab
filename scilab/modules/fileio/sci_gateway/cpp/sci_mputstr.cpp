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
#include "bool.hxx"
#include "function.hxx"

extern "C"
{
#include "mputl.h"
#include "localization.h"
#include "Scierror.h"
#include "charEncoding.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_mputstr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iFile               = -1; //default file : last opened file
    types::String* pString  = NULL;
    int iErr                = 1;

    types::Double* pdFileId = NULL;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "mputstr", 1, 2);
        return types::Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "mputstr", 1);
        return types::Function::Error;
    }

    pString = in[0]->getAs<types::String>();

    if (in.size() == 2)
    {
        if (in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false || in[1]->getAs<types::Double>()->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "mputstr", 2);
            return types::Function::Error;
        }
        iFile = static_cast<int>(in[1]->getAs<types::Double>()->get(0));
    }

    switch (iFile)
    {
        case 5: // stdin
            Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mputstr", iFile);
            return types::Function::Error;
        default :
            iErr = mputl(iFile, pString->get(), 1, FALSE);
    }

    out.push_back(new types::Bool(!iErr));

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
