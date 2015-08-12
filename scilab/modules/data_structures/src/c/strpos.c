/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
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
            if (memcmp(str, &chars[k - 1], sizeof(int) * (*n)) == 0)
            {
                return i;
            }
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
