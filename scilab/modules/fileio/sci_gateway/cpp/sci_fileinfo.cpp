/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_fileinfo(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "fileinfo", 1);
        return Function::Error;
    }

    if (in[0]->isString() == false )
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "fileinfo", 1);
        return Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output arguments: %d or %d expected.\n"), "fileinfo", 1, 2);
        return Function::Error;
    }

    String* pS = in[0]->getAs<types::String>();
    InternalType* iT = NULL;
    String* pS1 = NULL;

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
        out.push_back(Double::Empty());
    }
    else
    {
        Double *pOut = new Double(pS->getRows(), FILEINFO_ARRAY_SIZE);
        pOut->set(pData);
        out.push_back(pOut);
    }

    if (_iRetCount == 2)
    {
        Double* pErr = new Double(pS->getRows(), 1);
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
    return Function::OK;
}
/*--------------------------------------------------------------------------*/

