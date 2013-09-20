/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "MALLOC.h"
#include "api_scilab.h"
#include "gw_io.h"
#include "stack-c.h"
#include "systemc.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_host(char *fname, unsigned long fname_len)
{
    SciErr sciErr;

    int *piAddr1;
    int *piLen;
    int i;
    int iRet;

    int m1 = 0, n1 = 0;
    char **Str = NULL;
    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }//
    if (isStringType(pvApiCtx, piAddr1))
    {

        //fisrt call to retrieve dimensions
        sciErr = getMatrixOfString(pvApiCtx, piAddr1, &m1, &n1, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        piLen = (int*)MALLOC(sizeof(int) * m1 * n1);

        //second call to retrieve length of each string
        sciErr = getMatrixOfString(pvApiCtx, piAddr1, &m1, &n1, piLen, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        Str = (char**)MALLOC(sizeof(char*) * m1 * n1);
        for (i = 0 ; i < m1 * n1 ; i++)
        {
            Str[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }

        //third call to retrieve data
        sciErr = getMatrixOfString(pvApiCtx, piAddr1, &m1, &n1, piLen, Str);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        if ( (m1 != 1) && (n1 != 1) )
        {
            freeArrayOfString(Str, m1 * n1);
            Scierror(89, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
            return 1;
        }
        else
        {
            int stat = 0;
            C2F(systemc)(Str[0], &stat);
            /* Create the matrix as return of the function */
            iRet = createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, stat);
            if (iRet)
            {
                return 1;
            }
            freeArrayOfString(Str, m1 * n1);

            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            ReturnArguments(pvApiCtx);
        }
    }
    else
    {
        Scierror(55, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
