/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#define __USE_DEPRECATED_STACK_FUNCTIONS__
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "freeArrayOfString.h"
#include "MALLOC.h"
#include "getCommonPart.h"
/*--------------------------------------------------------------------------*/
int sci_getcommonpart(char *fname, unsigned long fname_len)
{
    CheckRhs(1, 1);
    CheckLhs(1, 1);

    if (GetType(1) == sci_strings)
    {
        int m = 0, n = 0;
        char **InputString = NULL;
        char *Output = NULL;

        GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &m, &n, &InputString);
        if ( ( (m == 1) && (n != 1) ) || ( (m != 1) && (n == 1) ) )
        {
            Output = getCommonPart(InputString, m * n);
            freeArrayOfString(InputString, m * n);

            if (Output == NULL)
            {
                int l = 0;
                m = 0, n = 0;
                CreateVar(Rhs + 1, STRING_DATATYPE,  &m, &n, &l);
            }
            else
            {
                n = 1;
                CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, (m = (int)strlen(Output), &m), &n, &Output);
                if (Output)
                {
                    FREE(Output);
                    Output = NULL;
                }
            }

            LhsVar(1) = Rhs + 1;
        }
        else
        {
            freeArrayOfString(InputString, m * n);
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 1);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input arguments.\n"), fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
