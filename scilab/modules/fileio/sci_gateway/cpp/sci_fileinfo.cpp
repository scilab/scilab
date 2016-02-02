/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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

#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "fileinfo.h"
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_fileinfo(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "fileinfo", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "fileinfo", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output arguments: %d or %d expected.\n"), "fileinfo", 1, 2);
        return types::Function::Error;
    }

    types::String* pS = in[0]->getAs<types::String>();
    types::InternalType* iT = NULL;
    types::String* pS1 = NULL;

    if (pS->getCols() != 1)
    {
        pS->transpose(iT);
        pS = iT->getAs<types::String>();
    }
    int *piErr = new int[pS->getRows()];
    double* pData = filesinfoW(pS->get(), pS->getRows(), piErr);

    /* fileinfo returns [] for single non existing file to match the behavior
       of Scilab < 6 */
    if (pS->getRows() == 1 && *piErr == -1)
    {
        out.push_back(types::Double::Empty());
    }
    else
    {
        types::Double *pOut = new types::Double(pS->getRows(), FILEINFO_ARRAY_SIZE);
        pOut->set(pData);
        out.push_back(pOut);
    }

    if (_iRetCount == 2)
    {
        types::Double* pErr = new types::Double(pS->getRows(), 1);
        pErr->setInt(piErr);
        out.push_back(pErr);
    }


    if (pS != in[0])
    {
        //in case of transpose
        pS->killMe();
    }

    delete[] piErr;
    FREE(pData);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

