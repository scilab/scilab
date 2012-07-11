/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include "mgetl.h"
#include "localization.h"
#include "Scierror.h"
#include "mopen.h"
#include "mclose.h"
#include "expandPathVariable.h"
}

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_mgetl(typed_list &in, int _iRetCount, typed_list &out)
{
    int iFileID                 = 0;
    int iErr                    = 0;
    bool bCloseFile             = false;
    int iLinesExcepted          = -1;
    int iLinesRead              = -1;
    wchar_t** wcReadedStrings   = NULL;

    if(in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "mgetl" , 1, 2);
        return Function::OK;
    }

    if(in.size() == 2)
    {//number of lines
        if(in[1]->isDouble() == false || in[1]->getAs<Double>()->getSize() != 1)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"), "mgetl", 2);
            return Function::Error;
        }
        iLinesExcepted = static_cast<int>(in[1]->getAs<Double>()->get(0));
    }

    if(in[0]->isDouble() && in[0]->getAs<Double>()->getSize() == 1)
    {
        iFileID = static_cast<int>(in[0]->getAs<Double>()->get(0));
    }
    else if(in[0]->isString() && in[0]->getAs<types::String>()->getSize() == 1)
    {
        wchar_t *expandedFileName = expandPathVariableW(in[0]->getAs<types::String>()->get(0));

        iErr = mopen(expandedFileName, L"rt", 0, &iFileID);

        if(iErr)
        {
            switch(iErr)
            {
            case MOPEN_NO_MORE_LOGICAL_UNIT:
                ScierrorW(66, _W("%ls: Too many files opened!\n"), L"mgetl");
                break;
            case MOPEN_CAN_NOT_OPEN_FILE:
                ScierrorW(999, _W("%ls: Cannot open file %ls.\n"), L"mgetl", expandedFileName);
                break;
            case MOPEN_NO_MORE_MEMORY:
                ScierrorW(999, _W("%ls: No more memory.\n"), L"mgetl");
                break;
            case MOPEN_INVALID_FILENAME:
                ScierrorW(999, _W("%ls: invalid filename %ls.\n"), L"mgetl", expandedFileName);
                break;
            default: //MOPEN_INVALID_STATUS
                ScierrorW(999, _W("%ls: invalid status.\n"), L"mgetl");
                break;
            }
            return Function::Error;
        }
        FREE(expandedFileName);
        bCloseFile = true;
    }
    else
    {//Error
        Scierror(999,_("%s: Wrong type for input argument #%d: a String or Integer expected.\n"), "mgetl", 1);
        return Function::Error;
    }

    switch (iFileID)
    {
        case 0: // stderr
        case 6: // stdout
            ScierrorW(999, _W("%ls: Wrong file descriptor: %d.\n"), L"mgetl", iFileID);
            return types::Function::Error;
        default :
        {
            wcReadedStrings = mgetl(iFileID, iLinesExcepted, &iLinesRead, &iErr);

            switch(iErr)
            {
            case MGETL_MEMORY_ALLOCATION_ERROR :
                break;

            }
        }
    }

    if(wcReadedStrings && iLinesRead > 0)
    {
        String *pS = new String(iLinesRead, 1);
        pS->set(wcReadedStrings);
        out.push_back(pS);
    }
    else
    {
        out.push_back(types::Double::Empty());
    }

    if(bCloseFile)
    {
        mclose(iFileID);
    }

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
