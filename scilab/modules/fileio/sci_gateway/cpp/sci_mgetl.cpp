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

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "mgetl" , 1, 2);
        return Function::OK;
    }

    if (in.size() == 2)
    {
        //number of lines
        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Integer expected.\n"), "mgetl", 2);
            return Function::Error;
        }

        if (in[1]->getAs<Double>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Integer expected.\n"), "mgetl", 2);
            return Function::Error;
        }
        iLinesExcepted = static_cast<int>(in[1]->getAs<Double>()->get(0));
    }

    if (in[0]->isDouble() && in[0]->getAs<Double>()->getSize() == 1)
    {
        iFileID = static_cast<int>(in[0]->getAs<Double>()->get(0));
    }
    else if (in[0]->isString() && in[0]->getAs<types::String>()->getSize() == 1)
    {
        wchar_t *expandedFileName = expandPathVariableW(in[0]->getAs<types::String>()->get(0));

        iErr = mopen(expandedFileName, L"rt", 0, &iFileID);

        if (iErr)
        {
            char* pst = wide_string_to_UTF8(expandedFileName);
            switch (iErr)
            {
                case MOPEN_NO_MORE_LOGICAL_UNIT:
                    Scierror(66, _("%s: Too many files opened!\n"), "mgetl");
                    break;
                case MOPEN_CAN_NOT_OPEN_FILE:
                    Scierror(999, _("%s: Cannot open file %s.\n"), "mgetl", pst);
                    break;
                case MOPEN_NO_MORE_MEMORY:
                    Scierror(999, _("%s: No more memory.\n"), "mgetl");
                    break;
                case MOPEN_INVALID_FILENAME:
                    Scierror(999, _("%s: invalid filename %s.\n"), "mgetl", pst);
                    break;
                default: //MOPEN_INVALID_STATUS
                    Scierror(999, _("%s: invalid status.\n"), "mgetl");
                    break;
            }

            FREE(pst);
            return Function::Error;
        }
        FREE(expandedFileName);
        bCloseFile = true;
    }
    else
    {
        //Error
        Scierror(999, _("%s: Wrong type for input argument #%d: a String or Integer expected.\n"), "mgetl", 1);
        return Function::Error;
    }

    switch (iFileID)
    {
        case 0: // stderr
        case 6: // stdout
            Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mgetl", iFileID);
            return types::Function::Error;
        default :
        {
            types::File* pFile = FileManager::getFile(iFileID);
            // file opened with fortran open function
            if (pFile == NULL || pFile->getFileType() == 1)
            {
                Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mgetl", iFileID);
                return types::Function::Error;
            }

            wcReadedStrings = mgetl(iFileID, iLinesExcepted, &iLinesRead, &iErr);

            switch (iErr)
            {
                case MGETL_MEMORY_ALLOCATION_ERROR :
                    break;

            }
        }
    }

    if (wcReadedStrings && iLinesRead > 0)
    {
        String *pS = new String(iLinesRead, 1);
        pS->set(wcReadedStrings);
        out.push_back(pS);
    }
    else
    {
        out.push_back(types::Double::Empty());
    }

    if (bCloseFile)
    {
        mclose(iFileID);
    }

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
