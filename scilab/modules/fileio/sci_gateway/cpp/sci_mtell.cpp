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
#include "localization.h"
#include "Scierror.h"
#include "mtell.h"
}

types::Function::ReturnValue sci_mtell(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iFile           = -1; //default file : last opened file
    int dims            = 2;
    int dimsArray[2]    = {1, 1};
    types::Double* pOut = NULL;

    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "mtell", 0, 1);
        return types::Function::Error;
    }
    if (in.size() == 1)
    {
        if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A Real expected.\n"), "mtell", 1);
            return types::Function::Error;
        }

        iFile = static_cast<int>(in[0]->getAs<types::Double>()->get(0));
    }

    switch (iFile)
    {
        case 0: // stderr
        case 5: // stdin
        case 6: // stdout
            Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mtell", iFile);
            return types::Function::Error;
    }

    long long offset = mtell(iFile);
    if (offset < 0)
    {
        Scierror(999, _("%s: Error while opening, reading or writing.\n"), "mtell");
        return types::Function::Error;
    }

    pOut = new types::Double(dims, dimsArray);
    pOut->set(0, (double)offset);

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
