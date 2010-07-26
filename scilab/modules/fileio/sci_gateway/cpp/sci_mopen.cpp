/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#include "alltypes.hxx"
#include "funcmanager.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"
extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "expandPathVariable.h"
#include "mopen.h"
#include "charEncoding.h"
#include "MALLOC.h"
}

using namespace types;

Function::ReturnValue sci_mopen(typed_list &in, int _iRetCount, typed_list &out)
{
    int iErr                = 0;
    int iID                 = 0;
    wchar_t* pstFilename    = NULL;
    wchar_t* pstMode        = L"wb";
    int iSwap               = 0;

    //check input parameters
    if(in.size() >= 1)
    {//filename
        if(in[0]->getType() != InternalType::RealString)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "mopen", 1);
            return Function::Error;
        }

        String* pS1 = in[0]->getAsString();
        if(pS1->size_get() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), "mopen" , 1);
            return Function::Error;
        }

        pstFilename = expandPathVariableW(pS1->string_get(0));

        if(in.size() >= 2)
        {//mode
            if(in[1]->getType() != InternalType::RealString)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "mopen", 2);
                return Function::Error;
            }

            String* pS2 = in[1]->getAsString();
            if(pS2->size_get() != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), "mopen" , 2);
                return Function::Error;
            }

            pstMode = pS2->string_get(0);

            if(in.size() >= 3)
            {//swap
                if(in[2]->getType() != InternalType::RealDouble)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), "mopen" , 3);
                    return Function::Error;
                }

                Double* pD3 = in[2]->getAsDouble();
                if(pD3->size_get() != 1 || pD3->isComplex())
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: An integer expected.\n"), "mopen", 3);
                    return Function::Error;
                }

                //if value == 0 set swap to 0 otherwise let to 1
                if(pD3->real_get(0,0) == 0)
                {
                    iSwap = 0;
                }

                if(in.size() >= 4)
                {
                    Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "mopen" , 1, 3);
                    return Function::Error;
                }

            }
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "mopen" , 1, 3);
        return Function::Error;
    }


    iErr = mopen(pstFilename, pstMode, iSwap, &iID);


    if(iErr != MOPEN_NO_ERROR)
    {//mange file open errors
        if(_iRetCount == 1)
        {
            switch(iErr)
            {
            case MOPEN_CAN_NOT_OPEN_FILE:
                {
                    ScierrorW(999, _W("%ls: Cannot open file %ls.\n"), L"mopen", pstFilename);
                    FREE(pstFilename);
                    pstFilename = NULL;
                    return Function::Error;
                }
            case MOPEN_INVALID_FILENAME:
                {
                    ScierrorW(999,_W("%ls: invalid filename.\n"), L"mopen");
                    FREE(pstFilename);
                    pstFilename = NULL;
                    return Function::Error;
                }
            case MOPEN_INVALID_STATUS:
                {
                    ScierrorW(999,_W("%ls: invalid status.\n"), L"mopen");
                    FREE(pstFilename);
                    pstFilename = NULL;
                    return Function::Error;
                }
            }
        }
    }

    Double* pD = new Double(static_cast<double>(iID));
    out.push_back(pD);
    return Function::OK;
}
