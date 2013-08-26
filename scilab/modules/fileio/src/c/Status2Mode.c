/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
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
#include "Status2Mode.h"
/*--------------------------------------------------------------------------*/
int Status2Mode(char *status)
{
    int bin, plus, md, n, k;
    /* default values */
    bin = 0;
    plus = 0;
    md = 0;
    n = (int)strlen(status);
    for (k = 0; k < n; k++)
    {
        if (status[k] == 'r')
        {
            md = 1;
        }
        else if (status[k] == 'w')
        {
            md = 2;
        }
        else if (status[k] == 'a')
        {
            md = 3;
        }
        else if (status[k] == '+')
        {
            plus = 1;
        }
        else if (status[k] == 'b')
        {
            bin = 1;
        }
    }
    return(100 * md + 10 * plus + bin);
}
/*--------------------------------------------------------------------------*/
