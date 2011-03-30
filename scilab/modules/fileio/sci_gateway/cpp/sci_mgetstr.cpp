/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include <stdio.h>
#include "localization.h"
#include "Scierror.h"
#include "charEncoding.h"
#include "mgetstr.h"
}
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_mgetstr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iDims                   = 2;
    int iDimsArray[2]           = {1,1};
    int iFile                   = -1; //default file : last opened file
    types::String* pOutString   = NULL;
    int iSizeToRead             = 0;
    types::Double* pdFileId     = NULL;
    
    if(in.size() < 1 || in.size() > 2)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"mgetstr", 1, 2);
        return types::Function::Error;
    }

    if(in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A real expected.\n"), L"mgetstr", 1);
        return types::Function::Error;
    }
    
    iSizeToRead = static_cast<int>(in[0]->getAs<types::Double>()->getReal()[0]);
    
    if(in.size() == 2)
    {
        if(in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false || in[1]->getAs<types::Double>()->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A real expected.\n"), L"mgetstr", 2);
            return types::Function::Error;
        }
        pdFileId = in[1]->getAs<types::Double>();
    }

    pOutString = new types::String(iDims,iDimsArray);

    if(pdFileId != NULL)
    {
        iFile = static_cast<int>(pdFileId->getReal()[0]);
    }
    
    wchar_t* pwstOut = mgetstr(iFile, iSizeToRead);
    if(pwstOut == NULL)
    {
 //       ScierrorW(999, _W("%ls: Unable to read file %d.\n"), L"mgetstr", iFile);
//        return types::Function::Error;
    }
    else
    {
        pOutString->set(0, pwstOut);
    }
    
    FREE(pwstOut);
    out.push_back(pOutString);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
