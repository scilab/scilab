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
#include "int.hxx"
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

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "mput", 1, 3);
        return Function::Error;
    }

    //check parameter 1
    if ((in[0]->isDouble() == false) && (in[0]->isInt() == false))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A integer expected.\n"), "mput", 1);
        return Function::Error;
    }

    iSize = in[0]->getAs<GenericType>()->getSize();

    if (in.size() >= 2)
    {
        //export format
        if (in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "mput", 2);
            return Function::Error;
        }
        pstType = wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));

    }

    if (in.size() == 3)
    {
        if (in[2]->isDouble() == false || in[2]->getAs<Double>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A integer expected.\n"), "mput", 3);
            return Function::Error;
        }

        iFile = static_cast<int>(in[2]->getAs<Double>()->get(0));
    }

    switch (iFile)
    {
        case 5: // stdin
            Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mput", iFile);
            return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        pData = in[0]->getAs<Double>()->get();
    }
    else
    {
        pData = (double*)malloc(iSize * sizeof(double));
        if (in[0]->isInt8())
        {
            char* piData = in[0]->getAs<Int8>()->get();
            for (int i = 0; i < iSize; i++)
            {
                pData[i] = (double)piData[i];
            }
        }
        else if (in[0]->isUInt8())
        {
            unsigned char* piData = in[0]->getAs<UInt8>()->get();
            for (int i = 0; i < iSize; i++)
            {
                pData[i] = (double)piData[i];
            }
        }
        else if (in[0]->isInt16())
        {
            short* piData = in[0]->getAs<Int16>()->get();
            for (int i = 0; i < iSize; i++)
            {
                pData[i] = (double)piData[i];
            }
        }
        else if (in[0]->isUInt16())
        {
            unsigned short* piData = in[0]->getAs<UInt16>()->get();
            for (int i = 0; i < iSize; i++)
            {
                pData[i] = (double)piData[i];
            }
        }
        else if (in[0]->isInt32())
        {
            int* piData = in[0]->getAs<Int32>()->get();
            for (int i = 0; i < iSize; i++)
            {
                pData[i] = (double)piData[i];
            }
        }
        else if (in[0]->isUInt32())
        {
            unsigned int* piData = in[0]->getAs<UInt32>()->get();
            for (int i = 0; i < iSize; i++)
            {
                pData[i] = (double)piData[i];
            }
        }
        else if (in[0]->isInt64())
        {
            long long* piData = in[0]->getAs<Int64>()->get();
            for (int i = 0; i < iSize; i++)
            {
                pData[i] = (double)piData[i];
            }
        }
        else if (in[0]->isUInt64())
        {
            unsigned long long* piData = in[0]->getAs<UInt64>()->get();
            for (int i = 0; i < iSize; i++)
            {
                pData[i] = (double)piData[i];
            }
        }
    }

    C2F(mput)(&iFile, pData, &iSize, pstType, &iErr);

    FREE(pstType);
    if (in[0]->isDouble() == false)
    {
        free(pData);
    }

    if (iErr)
    {
        Scierror(10000, "");
        return types::Function::Error;
    }

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
