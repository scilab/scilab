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
#include "dynlib_data_structures.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
DATA_STRUCTURES_IMPEXP int C2F(strpos)(const int *ptr, const int *ns, const int *chars, const int *str, const int *n)
{
    int  i = 0;

    for (i = 1; i <= *ns; ++i) 
    {
        int k = ptr[i - 1];
        int l = ptr[i];

        if ((l - k) == *n) 
        {
            int j = 0;
            int doReturn = 1;
            for (j = 1; j <= *n; ++j) 
            {
                if (str[j - 1] != chars[k - 2 + j]) 
                {
                    doReturn = 0;
                    break;
                }
            }
            if (doReturn) return i;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
