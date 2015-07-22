/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Kartik Gupta
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "gw_fileio.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
#include "freeArrayOfString.h"
#include "expandPathVariable.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include <unistd.h>
#include "isdir.h"
#include "createdirectory.h"
#include "scicurdir.h"
#include "archive_compress.h"
#include "dlManager.h"
/*--------------------------------------------------------------------------*/
int sci_postURL(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    char *pStVarOne = NULL;
    char *pStVarOneExpanded = NULL;

    int ret = 0;
    int *piAddressVarTwo = NULL;
    char **pStVarTwo = NULL;
    int m1 = 0;
    int n1 = 0;
    int i = 0;

    int *piAddressVarThree = NULL;
    char **pStVarThree = NULL;
    int m2 = 0;
    int n2 = 0;

    char *username = NULL;
    char *password = NULL;
    char *dest = NULL;
    CheckRhs(3, 5);
    CheckLhs(1, 2);
    int result = 0;
    int *error = 0;
    int iRhs = nbInputArgument(pvApiCtx);
    char *filePath = NULL;
    char *content;
    char *url;

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne) == 0 || isScalar(pvApiCtx, piAddressVarOne) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }


    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddressVarTwo))
    {
        if (isEmptyMatrix(pvApiCtx, piAddressVarTwo))
        {
            if (createEmptyMatrix(pvApiCtx, Rhs + 2))
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 1);
            return 0;
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarTwo) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarTwo, &m1, &n1, &pStVarTwo))
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

      sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddressVarThree))
    {
        if (isEmptyMatrix(pvApiCtx, piAddressVarThree))
        {
            if (createEmptyMatrix(pvApiCtx, Rhs + 3))
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 3);
            return 0;
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarThree) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 3);
        return 0;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarThree, &m2, &n2, &pStVarThree))
    {
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    if(m1*n1 != m2*n2)
    {
	    Scierror(999, _("%s: Sorry the number of post values are not equal to number of post arguments\n"), fname);
    }

    if (iRhs > 3)
    {
	    int *piAddressVarFour = NULL;
	    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
	    if (sciErr.iErr)
	    {
		    Scierror(999, _("%s:Sorry memory allocation error \n"), fname);
		    return 0;
	    }
	    ret = getAllocatedSingleString(pvApiCtx, piAddressVarFour, &dest);
	    if (ret)
	    {
		    Scierror(999, _("%s:Sorry memory allocation error \n"), fname);
		    return 0;
	    }

    }
   
    if (iRhs > 4)
    {
	    int *piAddressVarFive = NULL;
	    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddressVarFive);
	    if (sciErr.iErr)
	    {
		    Scierror(999, _("%s:Sorry memory allocation error \n"), fname);
		    return 0;
	    }
	    ret = getAllocatedSingleString(pvApiCtx, piAddressVarFive, &username);
	    if(ret)
	    {
		    Scierror(999, _("%s:Sorry memory allocation error \n"), fname);
		    return 0;
	    }
    }

    if (iRhs > 5)
    {
	    int *piAddressVarSix = NULL;
	    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddressVarSix);
	    if (sciErr.iErr)
	    {
		    Scierror(999, _("%s:Sorry memory allocation error \n"), fname);
		    return 0;
	    }
	    ret = getAllocatedSingleString(pvApiCtx, piAddressVarSix, &password);
	    if(ret)
	    {
		    Scierror(999, _("%s:Sorry memory allocation error \n"), fname);
		    return 0;
	    }
    }


    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &url))
    {
        if (pStVarOne)
        {
            freeAllocatedSingleWideString(pStVarOne);
        }

        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    pStVarTwo[m1*n1] = NULL;
    pStVarThree[m1*n1] = NULL;
    result = 1;
    filePath = postFile(url, pStVarTwo,pStVarThree,dest, username, password, &content);
    if (filePath)
    {
        //Return the second argument which si the file name
        ret = createSingleString(pvApiCtx, iRhs + 1, filePath);
        FREE(filePath);
        filePath = NULL;
        if (ret)
        {
            Scierror(999, _("%s: Could not create the output argument.\n"));
            return 0;
        }
    }

    if (content && nbOutputArgument(pvApiCtx) == 2)
    {
        //create new variable with the content
        int res = createSingleString(pvApiCtx, iRhs + 2, content);
        if (res)
        {
            Scierror(999, _("%s: Could not create the output argument.\n"));
            return 0;
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;
    AssignOutputVariable(pvApiCtx, 2) = iRhs + 2;
    ReturnArguments(pvApiCtx);

    freeAllocatedSingleString(pStVarOne);
    freeAllocatedSingleString(pStVarTwo);
    return 0;

}
/*--------------------------------------------------------------------------*/


