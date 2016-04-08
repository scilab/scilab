/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

types::Function::ReturnValue sci_mopen(types::typed_list &in, int _iRetCount, types::typed_list &out)
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
        return types::Function::Error;
    }

    //check input parameters
    if (in.size() >= 1)
    {
        //filename
        if (in[0]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "mopen", 1);
            return types::Function::Error;
        }

        types::String* pS1 = in[0]->getAs<types::String>();
        if (pS1->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), "mopen" , 1);
            return types::Function::Error;
        }

        pstFilename = expandPathVariableW(pS1->get(0));

        if (in.size() >= 2)
        {
            //mode
            if (in[1]->isString() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "mopen", 2);
                FREE(pstFilename);
                return types::Function::Error;
            }

            types::String* pS2 = in[1]->getAs<types::String>();
            if (pS2->getSize() != 1)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), "mopen" , 2);
                FREE(pstFilename);
                return types::Function::Error;
            }

            pstMode = pS2->get(0);

            if (in.size() >= 3)
            {
                //swap
                if (in[2]->isDouble() == false)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), "mopen" , 3);
                    FREE(pstFilename);
                    return types::Function::Error;
                }

                types::Double* pD3 = in[2]->getAs<types::Double>();
                if (pD3->getSize() != 1 || pD3->isComplex())
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: An integer expected.\n"), "mopen", 3);
                    FREE(pstFilename);
                    return types::Function::Error;
                }

                //if value == 0 set swap to 0 otherwise let to 1
                if (pD3->getReal(0, 0) == 0)
                {
                    iSwap = 0;
                }

                if (in.size() >= 4)
                {
                    Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "mopen" , 1, 3);
                    FREE(pstFilename);
                    return types::Function::Error;
                }

            }
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "mopen" , 1, 3);
        return types::Function::Error;
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
                    return types::Function::Error;
                }
                case MOPEN_INVALID_FILENAME:
                {
                    Scierror(999, _("%s: invalid filename.\n"), "mopen");
                    FREE(pstFilename);
                    FREE(pwstTemp);
                    pstFilename = NULL;
                    return types::Function::Error;
                }
                case MOPEN_INVALID_STATUS:
                {
                    Scierror(999, _("%s: invalid status.\n"), "mopen");
                    FREE(pstFilename);
                    FREE(pwstTemp);
                    pstFilename = NULL;
                    return types::Function::Error;
                }
            }
        }
    }

    FREE(pwstTemp);
    FREE(pstFilename);

    types::Double* pD = new types::Double(static_cast<double>(iID));
    out.push_back(pD);

    if (_iRetCount == 2)
    {
        types::Double* pD2 = new types::Double(iErr);
        out.push_back(pD2);
    }
    return types::Function::OK;
}
