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
#include "mtell.h"
}


Function::ReturnValue sci_mtell(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iFile           = -1; //default file : last opened file
    int dims            = 2;
    int dimsArray[2]    = {1,1};
    types::Double* pOut = NULL;

    if(in.size() > 1)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"mtell", 0, 1);
        return types::Function::Error;
    }
    if(in.size() == 1)
    {
        if(in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A Real expected.\n"), L"mtell", 1);
            return types::Function::Error;
        }

        iFile = static_cast<int>(in[0]->getAs<types::Double>()->getReal()[0]);
    }

    long int offset = mtell(iFile);
    if(offset < 0)
    {
        ScierrorW(999, _W("%ls: Unable to read file %d.\n"), L"mtell", iFile);
        return types::Function::Error;
    }

    pOut = new types::Double(dims,dimsArray);
    pOut->set(0,offset);

    out.push_back(pOut);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
