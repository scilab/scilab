/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "isScilabFunction.h"
#include "searchmacroinlibraries.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "sci_types.h"
#include "stack-def.h"
#include "Funtab.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
BOOL isScilabFunction(const char * functionName)
{
    return (isScilabBuiltIn(functionName) || 
            isScilabMacroVariable(functionName) || 
            isScilabMacro(functionName));
}
/*--------------------------------------------------------------------------*/
BOOL isScilabMacro(const char * functionName)
{
    if (functionName)
    {
        char **librariesResult = NULL;
        int librariesResultSize = 0;

        librariesResult = searchmacroinlibraries((char*)functionName, &librariesResultSize);
        freeArrayOfString(librariesResult, librariesResultSize);
        if (librariesResultSize > 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL isScilabMacroVariable(const char * functionName)
{
    if (functionName)
    {
        int *piAddr = NULL;
        SciErr sciErr = getVarAddressFromName(pvApiCtx, functionName, &piAddr);
        if (sciErr.iErr == 0)
        {
            int typeVariable = 0;
            sciErr = getVarType(pvApiCtx, piAddr, &typeVariable);
            if (sciErr.iErr == 0)
            {
                return (BOOL)(typeVariable == sci_c_function);
            }
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL isScilabBuiltIn(const char * functionName)
{
    if (functionName)
    {
        int id[nsiz];
        int funptr = 0;
        int zero = 0;
        int one = 1;
        int job = one; /* Find function & returns fptr value */
        C2F(cvname)(id, (char*)functionName, &zero, (unsigned long)strlen(functionName));
        C2F(funtab)(id, &funptr, &job, "NULL_NAME", zero);
        return (BOOL)(funptr != zero);
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
