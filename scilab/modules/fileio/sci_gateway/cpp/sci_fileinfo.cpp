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
#include "alltypes.hxx"
#include "funcmanager.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"

extern "C"
{
#include "fileinfo.h"
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_fileinfo(typed_list &in, int _iRetCount, typed_list &out)
{
    if(in.size() != 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"fileinfo", 1);
        return Function::Error;
    }

    if(in[0]->isString() == false || in[0]->getAsString()->cols_get() != 1)
    {
        Scierror(999, _("%ls: Wrong type for input argument: Column vector expected.\n"), L"fileinfo");
        return Function::Error;
    }

    if(_iRetCount > 2)
    {
        ScierrorW(78, _W("%ls: Wrong number of output arguments: %d or %d expected.\n"), L"fileinfo", 1, 2);
        return Function::Error;
    }

    String* pS = in[0]->getAsString();

    int *piErr = new int[pS->rows_get()];
    double* pData = filesinfoW(pS->string_get(), pS->rows_get(), piErr);

    Double *pOut = new Double(pS->rows_get(), FILEINFO_ARRAY_SIZE);
    pOut->real_set(pData);
    out.push_back(pOut);

    if(_iRetCount == 2)
    {
        Double* pErr = new Double(pS->rows_get(), 1);
        pErr->real_set(piErr);
        out.push_back(pErr);
    }

    delete[] piErr;
    FREE(pData);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/

