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
#include "funcmanager.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"

extern "C"
{
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
    int iFileID         = 0;
    int iErr            = 0;
    bool bCloseFile     = false;
    int iLinesExcepted  = -1;
    int iLinesRead      = -1;

    if(in.size() < 1 || in.size() > 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d to %d expected.\n"), "mgetl" , 1, 2);
        return Function::OK;
    }

    if(in.size() == 2)
    {//number of lines
        if(in[1]->getType() != InternalType::RealDouble || in[1]->getAsDouble()->size_get() != 1)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"), "mgetl", 2);
            return Function::Error;
        }

        iLinesExcepted = static_cast<int>(in[1]->getAsDouble()->real_get()[0]);
    }

    if(in[0]->getType() == InternalType::RealDouble && in[0]->getAsDouble()->size_get() == 1)
    {
        iFileID = static_cast<int>(in[0]->getAsDouble()->real_get()[0]);
    }
    else if(in[0]->getType() == InternalType::RealString && in[0]->getAsString()->size_get() == 1)
    {
        wchar_t *expandedFileName = expandPathVariableW(in[0]->getAsString()->string_get(0));

        iErr = mopen(expandedFileName, L"rt", 0, &iFileID);
        FREE(expandedFileName);

        if(iErr)
        {
            switch(iErr)
            {
            case MOPEN_NO_MORE_LOGICAL_UNIT:
                ScierrorW(66, _W("%ls: Too many files opened!\n"), L"mputl");
                break;
            case MOPEN_CAN_NOT_OPEN_FILE:
                ScierrorW(999, _W("%ls: Cannot open file %ls.\n"), L"mputl", expandedFileName);
                break;
            case MOPEN_NO_MORE_MEMORY:
                ScierrorW(999, _W("%ls: No more memory.\n"), L"mputl");
                break;
            case MOPEN_INVALID_FILENAME:
                ScierrorW(999, _W("%ls: invalid filename %ls.\n"), L"mputl", expandedFileName);
                break;
            default: //MOPEN_INVALID_STATUS
                ScierrorW(999, _W("%ls: invalid status.\n"), L"mputl");
                break;
            }
            return Function::Error;
        }
        bCloseFile = true;
    }
    else
    {//Error
        Scierror(999,_("%s: Wrong type for input argument #%d: a String or Integer expected.\n"), "mgetl", 1);
        return Function::Error;
    }

    wchar_t** wcReadedStrings = mgetl(iFileID, iLinesExcepted, &iLinesRead, &iErr);

    switch(iErr)
    {
    case MGETL_MEMORY_ALLOCATION_ERROR : 
        break;

    }
    String *pS = new String(iLinesRead, 1);
    pS->string_set(wcReadedStrings);
    out.push_back(pS);

    if(bCloseFile)
    {
        mclose(iFileID);
    }

    return Function::OK;
    //SciErr sciErr;
    //int *piAddressVarOne = NULL;

    //Rhs = Max(0, Rhs);

    //CheckRhs(1, 2);
    //CheckLhs(1, 1);

    //if (Rhs == 2)
    //{
    //    int *piAddressVarTwo = NULL;

    //    sciErr = getVarAddressFromPosition(_piKey, 2, &piAddressVarTwo);
    //    if(sciErr.iErr)
    //    {
    //        printError(&sciErr, 0);
    //        return 0;
    //    }

    //    if ( isDoubleType(_piKey, piAddressVarTwo) )
    //    {
    //        double dValue = 0.;
    //        if (!isScalar(_piKey, piAddressVarTwo))
    //        {
    //            Scierror(999,_("%s: Wrong size for input argument #%d: Integer expected.\n"), fname, 2);
    //            return 0;
    //        }

    //        if ( getScalarDouble(_piKey, piAddressVarTwo, &dValue) == 0)
    //        {
    //            numberOfLinesToRead = (int)dValue;
    //        }
    //        else
    //        {
    //            Scierror(999,_("%s: Memory allocation error.\n"), fname);
    //            return 0;
    //        }
    //    }
    //    else
    //    {
    //        Scierror(999,_("%s: Wrong type for input argument #%d: Integer expected.\n"), fname, 2);
    //        return 0;
    //    }
    //}

    //sciErr = getVarAddressFromPosition(_piKey, 1, &piAddressVarOne);
    //if(sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 0;
    //}

    //if ( isStringType(_piKey, piAddressVarOne) || isDoubleType(_piKey, piAddressVarOne) )
    //{
    //    char **wcReadedStrings = NULL;
    //    int numberOfLinesReaded = 0;
    //    int fileDescriptor = -1;
    //    int iErrorMgetl = 0;
    //    BOOL bCloseFile = FALSE;

    //    if (!isScalar(_piKey, piAddressVarOne))
    //    {
    //        Scierror(999,_("%s: Wrong size for input argument #%d: String or logical unit expected.\n"), fname, 1);
    //        return 0;
    //    }

    //    if (isStringType(_piKey, piAddressVarOne))
    //    {
    //        char *fileName = NULL;
    //        if (getAllocatedSingleString(_piKey, piAddressVarOne, &fileName) == 0)
    //        {
    //            char *expandedFileName = expandPathVariable(fileName);
    //            freeAllocatedSingleString(fileName);
    //            fileName = NULL;

    //            if (IsAlreadyOpenedInScilab(expandedFileName))
    //            {
    //                int fd = GetIdFromFilename(expandedFileName);
    //                fileDescriptor = fd;
    //                if (expandedFileName) {FREE(expandedFileName); expandedFileName = NULL;}
    //                bCloseFile = FALSE;
    //            }
    //            else
    //            {
    //                #define READ_ONLY_TEXT_MODE "rt"
    //                int fd = 0;
    //                int f_swap = 0;
    //                double res = 0.0;
    //                int ierr = 0;

    //                C2F(mopen)(&fd, expandedFileName, READ_ONLY_TEXT_MODE, &f_swap, &res, &ierr);
    //                bCloseFile = TRUE;

    //                switch (ierr)
    //                {
    //                    case MOPEN_NO_ERROR:
    //                        fileDescriptor = fd;
    //                        if (expandedFileName) {FREE(expandedFileName); expandedFileName = NULL;}
    //                        break;
    //                    case MOPEN_NO_MORE_LOGICAL_UNIT:
    //                        {
    //                            Scierror(66, _("%s: Too many files opened!\n"), fname);
    //                            if (expandedFileName) {FREE(expandedFileName); expandedFileName = NULL;}
    //                            return 0;
    //                        }
    //                        break;
    //                    case MOPEN_CAN_NOT_OPEN_FILE:
    //                        {
    //                            Scierror(999, _("%s: Cannot open file %s.\n"), fname, expandedFileName);
    //                            if (expandedFileName) {FREE(expandedFileName); expandedFileName = NULL;}
    //                            return 0;
    //                        }
    //                        break;
    //                    case MOPEN_NO_MORE_MEMORY:
    //                        {
    //                            if (expandedFileName) {FREE(expandedFileName); expandedFileName = NULL;}
    //                            Scierror(999, _("%s: No more memory.\n"), fname);
    //                            return 0;
    //                        }
    //                        break;
    //                    case MOPEN_INVALID_FILENAME:
    //                        {
    //                            Scierror(999, _("%s: invalid filename %s.\n"), fname, expandedFileName);
    //                            if (expandedFileName) {FREE(expandedFileName); expandedFileName = NULL;}
    //                            return 0;
    //                        }
    //                        break;
    //                    case MOPEN_INVALID_STATUS: default:
    //                        {
    //                            if (expandedFileName) {FREE(expandedFileName); expandedFileName = NULL;}
    //                            Scierror(999, _("%s: invalid status.\n"), fname);
    //                            return 0;
    //                        }
    //                        break;
    //                }
    //            }
    //        }
    //        else
    //        {
    //            Scierror(999,_("%s: Memory allocation error.\n"), fname);
    //            return 0;
    //        }
    //    }
    //    else /* double */
    //    {
    //        double dValue = 0.;

    //        if ( !getScalarDouble(_piKey, piAddressVarOne, &dValue) )
    //        {
    //            FILE *fd = NULL;
    //            fileDescriptor = (int)dValue;
    //            if ((fileDescriptor == STDIN_ID) || (fileDescriptor == STDOUT_ID))
    //            {
    //                SciError(244);
    //                return 0;
    //            }

    //            fd = GetFileOpenedInScilab(fileDescriptor);
    //            if (fd == NULL)
    //            {
    //                Scierror(245,_("%s: No input file associated to logical unit %d.\n"), fname, fileDescriptor);
    //                return 0;
    //            }
    //        }
    //        else
    //        {
    //            Scierror(999,_("%s: Memory allocation error.\n"), fname);
    //            return 0;
    //        }
    //    }

    //    wcReadedStrings = mgetl(fileDescriptor, numberOfLinesToRead, &numberOfLinesReaded, &iErrorMgetl);

    //    if (bCloseFile)
    //    {
    //        double dErrClose = 0.;
    //        C2F(mclose)(&fileDescriptor, &dErrClose);
    //        bCloseFile = FALSE;
    //    }

    //    switch(iErrorMgetl)
    //    {
    //        case MGETL_NO_ERROR:
    //        {
    //            if (numberOfLinesReaded == 0)
    //            {
    //                if (createEmptyMatrix(_piKey, Rhs + 1) != 0)
    //                {
    //                    Scierror(999,_("%s: Memory allocation error.\n"), fname);
    //                    return 0;
    //                }
    //            }
    //            else
    //            {
    //                int m = numberOfLinesReaded;
    //                int n = 1;

    //                sciErr = createMatrixOfString(_piKey, Rhs + 1, m, n, wcReadedStrings);
    //                if(sciErr.iErr)
    //                {
    //                    printError(&sciErr, 0);
    //                    return 0;
    //                }
    //            }

    //            freeArrayOfString(wcReadedStrings, numberOfLinesReaded);
    //            wcReadedStrings = NULL;
    //        }
    //        break;

    //        case MGETL_EOF:
    //        {
    //            if (numberOfLinesReaded == 0)
    //            {
    //                if (createEmptyMatrix(_piKey, Rhs + 1) != 0)
    //                {
    //                    Scierror(999,_("%s: Memory allocation error.\n"), fname);
    //                    return 0;
    //                }
    //            }
    //            else
    //            {
    //                int m = numberOfLinesReaded;
    //                int n = 1;

    //                sciErr = createMatrixOfString(_piKey, Rhs + 1, m, n, wcReadedStrings);
    //                if(sciErr.iErr)
    //                {
    //                    printError(&sciErr, 0);
    //                    return 0;
    //                }
    //                freeArrayOfString(wcReadedStrings, numberOfLinesReaded);
    //                wcReadedStrings = NULL;
    //            }
    //        }
    //        break;

    //        case MGETL_MEMORY_ALLOCATION_ERROR:
    //        {
    //            if (wcReadedStrings)
    //            {
    //                freeArrayOfString(wcReadedStrings, numberOfLinesReaded);
    //                wcReadedStrings = NULL;
    //            }
    //            Scierror(999,_("%s: Memory allocation error.\n"), fname);
    //            return 0;
    //        }
    //        break;

    //        case MGETL_ERROR:
    //        {
    //            if (wcReadedStrings)
    //            {
    //                freeArrayOfString(wcReadedStrings, numberOfLinesReaded);
    //                wcReadedStrings = NULL;
    //            }
    //            Scierror(999,_("%s: error.\n"), fname);
    //            return 0;
    //        }
    //        break;
    //    }

    //    LhsVar(1) = Rhs + 1;
    //    C2F(putlhsvar)();
    //}
    //else
    //{
    //    Scierror(999,_("%s: Wrong type for input argument #%d: String or logical unit expected.\n"), fname, 1);
    //}

    //return 0;
}
/*--------------------------------------------------------------------------*/
