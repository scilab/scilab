/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "mgetl.h"
#include "localization.h"
#include "Scierror.h"
#include "mopen.h"
#include "mclose.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_mgetl(types::typed_list &in, int _iRetCount, types::typed_list &out)
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
        return types::Function::OK;
    }

    if (in.size() == 2)
    {
        //number of lines
        if (in[1]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), "mgetl", 2);
            return types::Function::Error;
        }

        if (in[1]->getAs<types::Double>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: An integer value expected.\n"), "mgetl", 2);
            return types::Function::Error;
        }

        if (in[1]->getAs<types::Double>()->get(0) != (int)in[1]->getAs<types::Double>()->get(0))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "mgetl", 2);
            return types::Function::Error;
        }

        iLinesExcepted = static_cast<int>(in[1]->getAs<types::Double>()->get(0));
    }

    if (in[0]->isDouble() && in[0]->getAs<types::Double>()->getSize() == 1)
    {
        iFileID = static_cast<int>(in[0]->getAs<types::Double>()->get(0));
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
            FREE(expandedFileName);
            return types::Function::Error;
        }
        FREE(expandedFileName);
        bCloseFile = true;
    }
    else
    {
        //Error
        Scierror(999, _("%s: Wrong type for input argument #%d: a String or Integer expected.\n"), "mgetl", 1);
        return types::Function::Error;
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
        types::String *pS = new types::String(iLinesRead, 1);
        pS->set(wcReadedStrings);
        out.push_back(pS);
        freeArrayOfWideString(wcReadedStrings, iLinesRead);
    }
    else
    {
        out.push_back(types::Double::Empty());
        if (wcReadedStrings)
        {
            FREE(wcReadedStrings);
        }
    }

    if (bCloseFile)
    {
        mclose(iFileID);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
