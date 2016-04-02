/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006/2007 - INRIA - Alan LAYEC
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
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

#include <string.h>
#include "api_scilab.h"
#include "callfftw.h"
#include "sci_malloc.h"
#include "gw_fftw.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/

/* Set fftw wisdom
*
* Scilab Syntax :
*   -->set_fftw_wisdom(txt);
*
* Input : a scilab string matrix
*
* Output : Nothing or an error messsage if
*          wisdom can't be read by fftw
*
*/
int sci_set_fftw_wisdom(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    char **Str1 = NULL;
    char *Str = NULL;
    int m1 = 0, n1 = 0;
    int len = 0, k = 0;
    int j = 0;
    int i = 0;
    int* piAddr1 = NULL;
    int* piLen = NULL;

    if (withMKL())
    {
        Scierror(999, _("%s: MKL fftw library does not implement wisdom functions yet.\n"), fname);
        return 0;
    }

    CheckInputArgument(pvApiCtx, 1, 1);

    //get variable address
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (isStringType(pvApiCtx, piAddr1) == 0)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 1;
    }

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
        free(piLen);
        printError(&sciErr, 0);
        return 1;
    }

    Str1 = (char**)MALLOC(sizeof(char*) * m1 * n1);
    for (i = 0 ; i < m1 * n1 ; i++)
    {
        Str1[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    }

    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &m1, &n1, piLen, Str1);
    if (sciErr.iErr)
    {
        free(piLen);
        freeArrayOfString(Str1, m1 * n1);
        printError(&sciErr, 0);
        return 1;
    }

    for (j = 0; j < m1 * n1; j++)
    {
        len += (int)strlen(Str1[j]) + 1;

        if (Str)
        {
            Str = (char *)REALLOC(Str, sizeof(char) * (len));
        }
        else
        {
            Str = (char *)MALLOC(sizeof(char) * (len));
        }

        if (Str == NULL)
        {
            freeArrayOfString(Str1, m1 * n1);
            free(piLen);
            Scierror(999, _("%s: Cannot allocate more memory.\n"), fname);
            return 1;
        }

        for (i = 0; i < (int)strlen(Str1[j]); i++)
        {
            Str[k + i] = Str1[j][i];
        }
        Str[k + strlen(Str1[j])] = '\n';
        k += (int)strlen(Str1[j]) + 1;
    }
    Str[k - 1] = '\0';

    free(piLen);
    freeArrayOfString(Str1, m1 * n1);

    if (!(call_fftw_import_wisdom_from_string(Str)))
    {
        FREE(Str);
        Str = NULL;
        Scierror(999, _("%s: Wrong value for input argument #%d: a valid wisdom expected.\n"), fname, 1);
        return 1;
    }
    FREE(Str);
    Str = NULL;

    AssignOutputVariable(pvApiCtx, 1) = 0;
    ReturnArguments(pvApiCtx);
    return 0;
}
/*--------------------------------------------------------------------------*/
