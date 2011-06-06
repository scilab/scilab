/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_dynamic_link.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"
#include "Scierror.h"
#include "dynamic_link.h"
#include "MALLOC.h"
#include "localization.h"
#include "dl_genErrorMessage.h"
#include "freeArrayOfString.h"
#include "os_strdup.h"
#include "os_wcsdup.h"
/*-----------------------------------------------------------------------------------*/
static int linkNoRhs(int *_piKey);
static int linkOneRhsShow(int *_piKey);
/*-----------------------------------------------------------------------------------*/
int sci_link(char *fname, int *_piKey)
{
    SciErr sciErr;
    BOOL fflag              = TRUE;
    int idsharedlibrary     = -1;
    char *SharedLibraryName = NULL;
    wchar_t param3flag      = L'f';

    char **pstSubName       = NULL;
    int iRowsSubName        = 0;
    int iColsSubName        = 0;
    int iSizeSubName        = 0;

    int returnedID          = -1;
    int iErr                = 0;

    CheckRhs(0,3);
    CheckLhs(1,1);

    if(Rhs == 0)
    {
        return linkNoRhs(_piKey);
    }
    else
    {
        if(Rhs >= 1)
        {
            int* piAddress1 = NULL;
            sciErr = getVarAddressFromPosition(_piKey, 1, &piAddress1);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            if(isDoubleType(_piKey, piAddress1))
            {
                double pdblReal = 0;
                if(getScalarDouble(_piKey, piAddress1, &pdblReal))
                {
                    Scierror(999, _("%s : Wrong value for argument #%d: %s\n"), fname, 1, _("Unique id of a shared library expected."));
                    return 1;
                }
            }
            else if(isStringType(_piKey, piAddress1))
            {
                if(getAllocatedSingleString(_piKey, piAddress1, &SharedLibraryName))
                {
                    FREE(SharedLibraryName);
                    return 1;
                }

                if((Rhs == 1) && (strcmp(SharedLibraryName,"show") == 0))
                {
                    return linkOneRhsShow(_piKey);
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A scalar or a string expected.\n"), fname);
                return 1;
            }
        }

        if(Rhs >= 2)
        {
            int* piAddress2 = NULL;
            sciErr = getVarAddressFromPosition(_piKey, 2, &piAddress2);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            //string scalar or vector
            if(isStringType(_piKey, piAddress2) && (isVector(_piKey, piAddress2) || isScalar(_piKey, piAddress2)))
            {
                if(getAllocatedMatrixOfString(_piKey, piAddress2, &iRowsSubName, &iColsSubName, &pstSubName))
                {
                    return 1;
                }

                iSizeSubName = iRowsSubName * iColsSubName;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A string or a string vector expected.\n"));
                return 1;
            }
        }

        if(Rhs == 3)
        {
            int* piAddress3     = NULL;
            wchar_t* pwstData   = NULL;
            int iRows           = 0;
            int iCols           = 0;

            sciErr = getVarAddressFromPosition(_piKey, 3, &piAddress3);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 1;
            }

            if(getAllocatedSingleWideString(_piKey, piAddress3, &pwstData))
            {
                return 1;
            }

            if((wcscmp(pwstData, L"f") == 0) || (wcscmp(pwstData, L"c") == 0))
            {
                if(pwstData[0] != L'f')
                {
                    fflag = FALSE;
                }
            }
            else
            {
                freeAllocatedSingleWideString(pwstData);
                Scierror(999,_("%s Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 3, "f", "c");
                return 0;
            }
        }


        returnedID = scilabLink(idsharedlibrary, SharedLibraryName, pstSubName, iSizeSubName, fflag, &iErr);
        if(iErr == 0)
        {
            if(createScalarDouble(_piKey, Rhs + 1, returnedID))
            {
                return 1;
            }
            LhsVar(1) = Rhs + 1;
            PutLhsVar();
        }
        else
        {
            dl_genErrorMessage(fname, iErr, SharedLibraryName);
        }

        if(Rhs >= 2)
        {
            freeArrayOfString(pstSubName, iSizeSubName);
        }

        if(SharedLibraryName)
        {
            FREE(SharedLibraryName);
            SharedLibraryName = NULL;
        }
    }

    return 0;
}
/*-----------------------------------------------------------------------------------*/
static int linkNoRhs(int* _piKey)
{
    SciErr sciErr;
    int iRet = 0;
    int sizeFunctionsList = 0;
    char ** FunctionsList = NULL;

    FunctionsList = getNamesOfFunctionsInSharedLibraries(&sizeFunctionsList);

    if( (FunctionsList) && (sizeFunctionsList > 0) )
    {
        sciErr = createMatrixOfString(_piKey, Rhs + 1, 1, sizeFunctionsList, FunctionsList);
        freeArrayOfString(FunctionsList,sizeFunctionsList);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
        LhsVar(1) = Rhs+1;
        PutLhsVar();
    }
    else
    {
        iRet = createEmptyMatrix(_piKey, Rhs + 1);
        if(iRet)
        {
            return iRet;
        }
        LhsVar(1) = Rhs+1;
        PutLhsVar();
    }
    return 0;
}
/*-----------------------------------------------------------------------------------*/
static int linkOneRhsShow(int *_piKey)
{
    SciErr sciErr;
    int iRet = 0;
    int m1 = 0, n1 = 0, l1 = 0;
    int *IdsList = NULL;
    int sizeIds = 0;

    ShowDynLinks();
    IdsList = getAllIdSharedLib(&sizeIds);

    if( (sizeIds>0) && (IdsList) )
    {
        sciErr = createMatrixOfInteger32(_piKey, Rhs + 1, 1, sizeIds, IdsList);
        if(IdsList)
        {
            FREE(IdsList);
            IdsList=NULL;
        }
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
    }
    else
    {
        iRet = createEmptyMatrix(_piKey, Rhs + 1);
        if(iRet)
        {
            return iRet;
        }
    }
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*-----------------------------------------------------------------------------------*/
