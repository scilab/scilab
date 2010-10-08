/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include <stdlib.h>
#include "MALLOC.h"
#include "stack-c.h"
#include "msgstore.h"
#include "lasterror.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int C2F(linestore)(int *n)
{
    setLastErrorLinePosition(*n);
    return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(funnamestore)(char *str, int *n, int lenstr)
{
    char *functionName = strdup(str);
    if ( (functionName) && (*n >= 0) )
    {
        functionName[*n] = 0;
        setLastErrorFunctionName(functionName);
    }

    if (functionName)
    {
        FREE(functionName);
        functionName = NULL;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(msgstore)(char *str, int *n)
{
    int iRes = 0;
    char *msg = strdup(str);
    if (msg)
    {
        msg[*n] = 0;
    }
    iRes = appendStringToLastErrorMessage(msg);

    if (msg)
    {
        FREE(msg);
        msg = NULL;
    }

    return iRes;
}
/*--------------------------------------------------------------------------*/
int GetLastErrorCode(void)
{
    return getLastErrorValue();
}
/*--------------------------------------------------------------------------*/

