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
#include "gw_fileio.h"
#include "localization.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "mgetl.h"
#include "mopen.h"
#include "mclose.h"
#include "expandPathVariable.h"
#include "filesmanagement.h"
#include "freeArrayOfString.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
int sci_mgetl(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int numberOfLinesToRead = -1;

    Rhs = Max(0, Rhs);

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    if (Rhs == 2)
    {
        int *piAddressVarTwo = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if ( isDoubleType(pvApiCtx, piAddressVarTwo) )
        {
            double dValue = 0.;
            if (!isScalar(pvApiCtx, piAddressVarTwo))
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: Integer expected.\n"), fname, 2);
                return 0;
            }

            if ( getScalarDouble(pvApiCtx, piAddressVarTwo, &dValue) == 0)
            {
                numberOfLinesToRead = (int)dValue;
            }
            else
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Integer expected.\n"), fname, 2);
            return 0;
        }
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if ( isStringType(pvApiCtx, piAddressVarOne) || isDoubleType(pvApiCtx, piAddressVarOne) )
    {
        char **wcReadedStrings = NULL;
        int numberOfLinesReaded = 0;
        int fileDescriptor = -1;
        int iErrorMgetl = 0;
        BOOL bCloseFile = FALSE;

        if (!isScalar(pvApiCtx, piAddressVarOne))
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String or logical unit expected.\n"), fname, 1);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarOne))
        {
            char *fileName = NULL;
            if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &fileName) == 0)
            {
                char *expandedFileName = expandPathVariable(fileName);
                freeAllocatedSingleString(fileName);
                fileName = NULL;

                if (IsAlreadyOpenedInScilab(expandedFileName))
                {
                    int fd = GetIdFromFilename(expandedFileName);
                    fileDescriptor = fd;
                    if (expandedFileName)
                    {
                        FREE(expandedFileName);
                        expandedFileName = NULL;
                    }
                    bCloseFile = FALSE;
                }
                else
                {
#define READ_ONLY_TEXT_MODE "rt"
                    int fd = 0;
                    int f_swap = 0;
                    double res = 0.0;
                    int ierr = 0;

                    C2F(mopen)(&fd, expandedFileName, READ_ONLY_TEXT_MODE, &f_swap, &res, &ierr);
                    bCloseFile = TRUE;

                    switch (ierr)
                    {
                        case MOPEN_NO_ERROR:
                            fileDescriptor = fd;
                            if (expandedFileName)
                            {
                                FREE(expandedFileName);
                                expandedFileName = NULL;
                            }
                            break;
                        case MOPEN_NO_MORE_LOGICAL_UNIT:
                        {
                            Scierror(66, _("%s: Too many files opened!\n"), fname);
                            if (expandedFileName)
                            {
                                FREE(expandedFileName);
                                expandedFileName = NULL;
                            }
                            return 0;
                        }
                        break;
                        case MOPEN_CAN_NOT_OPEN_FILE:
                        {
                            Scierror(999, _("%s: Cannot open file %s.\n"), fname, expandedFileName);
                            if (expandedFileName)
                            {
                                FREE(expandedFileName);
                                expandedFileName = NULL;
                            }
                            return 0;
                        }
                        break;
                        case MOPEN_NO_MORE_MEMORY:
                        {
                            if (expandedFileName)
                            {
                                FREE(expandedFileName);
                                expandedFileName = NULL;
                            }
                            Scierror(999, _("%s: No more memory.\n"), fname);
                            return 0;
                        }
                        break;
                        case MOPEN_INVALID_FILENAME:
                        {
                            Scierror(999, _("%s: invalid filename %s.\n"), fname, expandedFileName);
                            if (expandedFileName)
                            {
                                FREE(expandedFileName);
                                expandedFileName = NULL;
                            }
                            return 0;
                        }
                        break;
                        case MOPEN_INVALID_STATUS:
                        default:
                        {
                            if (expandedFileName)
                            {
                                FREE(expandedFileName);
                                expandedFileName = NULL;
                            }
                            Scierror(999, _("%s: invalid status.\n"), fname);
                            return 0;
                        }
                        break;
                    }
                }
            }
            else
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }
        else /* double */
        {
            double dValue = 0.;

            if ( !getScalarDouble(pvApiCtx, piAddressVarOne, &dValue) )
            {
                FILE *fd = NULL;
                fileDescriptor = (int)dValue;
                if ((fileDescriptor == STDIN_ID) || (fileDescriptor == STDOUT_ID))
                {
                    SciError(244);
                    return 0;
                }

                fd = GetFileOpenedInScilab(fileDescriptor);
                if (fd == NULL)
                {
                    Scierror(245, _("%s: No input file associated to logical unit %d.\n"), fname, fileDescriptor);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }

        wcReadedStrings = mgetl(fileDescriptor, numberOfLinesToRead, &numberOfLinesReaded, &iErrorMgetl);

        if (bCloseFile)
        {
            double dErrClose = 0.;
            C2F(mclose)(&fileDescriptor, &dErrClose);
        }

        switch (iErrorMgetl)
        {
            case MGETL_NO_ERROR:
            {
                if (numberOfLinesReaded == 0)
                {
                    if (createEmptyMatrix(pvApiCtx, Rhs + 1) != 0)
                    {
                        Scierror(999, _("%s: Memory allocation error.\n"), fname);
                        return 0;
                    }
                }
                else
                {
                    int m = numberOfLinesReaded;
                    int n = 1;

                    sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, m, n, wcReadedStrings);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(17, _("%s: Memory allocation error.\n"), fname);
                        return 0;
                    }
                }

                freeArrayOfString(wcReadedStrings, numberOfLinesReaded);
                wcReadedStrings = NULL;
            }
            break;

            case MGETL_EOF:
            {
                if (numberOfLinesReaded == 0)
                {
                    if (createEmptyMatrix(pvApiCtx, Rhs + 1) != 0)
                    {
                        Scierror(999, _("%s: Memory allocation error.\n"), fname);
                        return 0;
                    }
                }
                else
                {
                    int m = numberOfLinesReaded;
                    int n = 1;

                    sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, m, n, wcReadedStrings);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(17, _("%s: Memory allocation error.\n"), fname);
                        return 0;
                    }
                    freeArrayOfString(wcReadedStrings, numberOfLinesReaded);
                    wcReadedStrings = NULL;
                }
            }
            break;

            case MGETL_MEMORY_ALLOCATION_ERROR:
            {
                if (wcReadedStrings)
                {
                    freeArrayOfString(wcReadedStrings, numberOfLinesReaded);
                    wcReadedStrings = NULL;
                }
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
            break;

            case MGETL_ERROR:
            {
                if (wcReadedStrings)
                {
                    freeArrayOfString(wcReadedStrings, numberOfLinesReaded);
                    wcReadedStrings = NULL;
                }
                Scierror(999, _("%s: error.\n"), fname);
                return 0;
            }
            break;
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String or logical unit expected.\n"), fname, 1);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
