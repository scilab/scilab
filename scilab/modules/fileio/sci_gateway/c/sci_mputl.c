/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "mopen.h"
#include "mclose.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
#include "filesmanagement.h"
#include "MALLOC.h"
#include "BOOL.h"
#include "mputl.h"
/*--------------------------------------------------------------------------*/ 
int sci_mputl(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int *piAddressVarTwo = NULL;

    char **pStVarOne			= NULL;
    int *lenStVarOne			= NULL;
    int mOne = 0, nOne = 0;
    int mnOne = 0;

    char *filename = NULL;
    int fileDescriptor = -1;
    BOOL bCloseFile = FALSE;

    int i = 0;
    int mputlErr = MPUTL_ERROR;

    if (Rhs != 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, 2);
        return 0;
    }

    if (Lhs != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if ( isDoubleType(pvApiCtx, piAddressVarTwo) )
    {
        double dValue = 0.;

        if (!isScalar(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: Integer expected.\n"), fname, 2);
            return 0;
        }

        if ( getScalarDouble(pvApiCtx, piAddressVarTwo, &dValue) == 0)
        {
            fileDescriptor = (int)dValue;
        }
        else
        {
            Scierror(999,_("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    } 
    else if ( isStringType(pvApiCtx, piAddressVarTwo) )
    {
        if (!isScalar(pvApiCtx, piAddressVarTwo))
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: String expected.\n"), fname, 2);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &filename) == 0)
        {
            #define WRITE_ONLY_TEXT_MODE "wt"
            int f_swap = 0;
            double res = 0.0;
            int ierr = 0;
            char *expandedFileName = expandPathVariable(filename);
            
            C2F(mopen)(&fileDescriptor, expandedFileName, WRITE_ONLY_TEXT_MODE, &f_swap, &res, &ierr);
            if (expandedFileName) {FREE(expandedFileName); expandedFileName = NULL;}

            switch (ierr)
            {
            case MOPEN_NO_ERROR:
                bCloseFile = TRUE;
                break;
            case MOPEN_NO_MORE_LOGICAL_UNIT:
                {
                    freeAllocatedSingleString(filename);
                    Scierror(66, _("%s: Too many files opened!\n"), fname);
                    return 0;
                }
                break;
            case MOPEN_CAN_NOT_OPEN_FILE:
                {
                    Scierror(999, _("%s: Cannot open file %s.\n"), fname, filename);
                    freeAllocatedSingleString(filename);
                    return 0;
                }
                break;
            case MOPEN_NO_MORE_MEMORY:
                {
                    freeAllocatedSingleString(filename);
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }
                break;
            case MOPEN_INVALID_FILENAME:
                {
                    Scierror(999, _("%s: invalid filename %s.\n"), fname, filename);
                    freeAllocatedSingleString(filename);
                    return 0;
                }
                break;
            case MOPEN_INVALID_STATUS: default:
                {
                    freeAllocatedSingleString(filename);
                    Scierror(999, _("%s: invalid status.\n"), fname);
                    return 0;
                }
                break;
            }
            bCloseFile = TRUE;
            freeAllocatedSingleString(filename);
        }
        else
        {
            Scierror(999,_("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: a String or Integer expected.\n"), fname, 2);
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        return 0;
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&mOne, &nOne, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if ( !((mOne == 1) || (nOne == 1)) )
    {
        Scierror(999,_("%s: Wrong size for input argument #%d: A 1-by-n or m-by-1 array expected.\n"), fname, 1);
        return 0;
    }

    mnOne = mOne * nOne;

    lenStVarOne = (int*)MALLOC(sizeof(int) * mnOne);
    if (lenStVarOne == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&mOne, &nOne, lenStVarOne, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    pStVarOne = (char**) MALLOC(sizeof(char*) * mnOne);
    if (pStVarOne == NULL)
    {
        FREE(lenStVarOne); lenStVarOne = NULL;
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    for (i = 0; i < mnOne; i++)
    {
        pStVarOne[i] = (char*)MALLOC(sizeof(char) * (lenStVarOne[i] + 1));
        if (pStVarOne[i] == NULL)
        {
            freeArrayOfString(pStVarOne, i);
            if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &mOne, &nOne, lenStVarOne, pStVarOne);
    if (lenStVarOne) {FREE(lenStVarOne); lenStVarOne = NULL;}
    if(sciErr.iErr)
    {
        freeArrayOfString(pStVarOne, mnOne);
        printError(&sciErr, 0);
        return 0;
    }

    mputlErr = mputl(fileDescriptor, pStVarOne, mnOne);
    freeArrayOfString(pStVarOne, mnOne);

    if (bCloseFile)
    {
        double dErrClose = 0.;
        C2F(mclose)(&fileDescriptor, &dErrClose);
        bCloseFile = FALSE;
    }

    switch (mputlErr)
    {
    case MPUTL_NO_ERROR:
        createScalarBoolean(pvApiCtx, Rhs + 1, TRUE);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        break;

    case MPUTL_INVALID_FILE_DESCRIPTOR:
        // commented for compatiblity
        // Scierror(999, _("%s: invalid file descriptor.\n"), fname);
        // break;
    case MPUTL_ERROR:
    case MPUTL_NO_WRITE_RIGHT:
    default:
        createScalarBoolean(pvApiCtx, Rhs + 1, FALSE);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        break;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/ 
