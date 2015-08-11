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

#include "string.hxx"
#include "double.hxx"
#include "function.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"

extern "C"
{
#include <PATH_MAX.h>
#include "Scierror.h"
#include "localization.h"
#include "expandPathVariable.h"
#include "mopen.h"
#include "charEncoding.h"
#include "sci_malloc.h"
#include "fullpath.h"
}

using namespace types;

Function::ReturnValue sci_mopen(typed_list &in, int _iRetCount, typed_list &out)
{
    int iErr                = 0;
    int iID                 = 0;
    wchar_t* pstFilename    = NULL;
    const wchar_t* pstMode  = L"rb";
    int iSwap               = 0;

    //check output parameters
    if (_iRetCount != 1 && _iRetCount != 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "mopen", 1, 2);
        return Function::Error;
    }

    //check input parameters
    if (in.size() >= 1)
    {
        //filename
        if (in[0]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "mopen", 1);
            return Function::Error;
        }

        String* pS1 = in[0]->getAs<types::String>();
        if (pS1->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), "mopen" , 1);
            return Function::Error;
        }

        pstFilename = expandPathVariableW(pS1->get(0));

        if (in.size() >= 2)
        {
            //mode
            if (in[1]->isString() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "mopen", 2);
                return Function::Error;
            }

            String* pS2 = in[1]->getAs<types::String>();
            if (pS2->getSize() != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), "mopen" , 2);
                return Function::Error;
            }

            pstMode = pS2->get(0);

            if (in.size() >= 3)
            {
                //swap
                if (in[2]->isDouble() == false)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), "mopen" , 3);
                    return Function::Error;
                }

                Double* pD3 = in[2]->getAs<Double>();
                if (pD3->getSize() != 1 || pD3->isComplex())
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: An integer expected.\n"), "mopen", 3);
                    return Function::Error;
                }

                //if value == 0 set swap to 0 otherwise let to 1
                if (pD3->getReal(0, 0) == 0)
                {
                    iSwap = 0;
                }

                if (in.size() >= 4)
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

    wchar_t* pwstTemp = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX * 2));
    get_full_pathW(pwstTemp, (const wchar_t*)pstFilename, PATH_MAX * 2);
    iErr = mopen(pwstTemp, pstMode, iSwap, &iID);
    if (iErr != MOPEN_NO_ERROR)
    {
        //mange file open errors
        if (_iRetCount == 1)
        {
            switch (iErr)
            {
                case MOPEN_CAN_NOT_OPEN_FILE:
                {
                    char* pst = wide_string_to_UTF8(pstFilename);
                    Scierror(999, _("%s: Cannot open file %s.\n"), "mopen", pst);
                    FREE(pst);
                    FREE(pstFilename);
                    FREE(pwstTemp);
                    pstFilename = NULL;
                    return Function::Error;
                }
                case MOPEN_INVALID_FILENAME:
                {
                    Scierror(999, _("%s: invalid filename.\n"), "mopen");
                    FREE(pstFilename);
                    FREE(pwstTemp);
                    pstFilename = NULL;
                    return Function::Error;
                }
                case MOPEN_INVALID_STATUS:
                {
                    Scierror(999, _("%s: invalid status.\n"), "mopen");
                    FREE(pstFilename);
                    FREE(pwstTemp);
                    pstFilename = NULL;
                    return Function::Error;
                }
            }
        }
    }

    FREE(pwstTemp);
    FREE(pstFilename);

    Double* pD = new Double(static_cast<double>(iID));
    out.push_back(pD);

    if (_iRetCount == 2)
    {
        Double* pD2 = new Double(iErr);
        out.push_back(pD2);
    }
    return Function::OK;
}
