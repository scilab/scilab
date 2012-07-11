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
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "MALLOC.h"
#include "mput.h"
#include "localization.h"
#include "Scierror.h"
#include "charEncoding.h"
#include "os_strdup.h"
}
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_mput(typed_list &in, int _iRetCount, typed_list &out)
{
    char* pstType   = os_strdup("l");//default type value : long
    int iSize       = 0;
    int iFile       = -1; //default file : last opened file
    double* pData   = NULL;
    int iErr        = 0;

    if(in.size() < 1 || in.size() > 3)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"mput", 1, 3);
        return Function::Error;
    }

    //check parameter 1
    if((in[0]->isDouble() == false) && (in[0]->isInt() == false))
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A integer expected.\n"), L"mput", 1);
        return Function::Error;
    }

    iSize = in[0]->getAs<GenericType>()->getSize();

    if(in[0]->isDouble())
    {
        pData = in[0]->getAs<Double>()->get();
    }
    else
    {//ints
        //???
    }

    if(in.size() >= 2)
    {//export format
        if(in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"mput", 2);
            return Function::Error;
        }
        pstType = wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));

    }

    if(in.size() == 3)
    {
        if(in[2]->isDouble() == false || in[2]->getAs<Double>()->getSize() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A integer expected.\n"), L"mput", 3);
            return Function::Error;
        }

        iFile = static_cast<int>(in[2]->getAs<Double>()->get(0));
    }

    switch (iFile)
    {
    case 5: // stdin
        ScierrorW(999, _W("%ls: Wrong file descriptor: %d.\n"), L"mput", iFile);
        return types::Function::Error;
    }

    C2F(mput)(&iFile, pData, &iSize, pstType, &iErr);

    FREE(pstType);
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
