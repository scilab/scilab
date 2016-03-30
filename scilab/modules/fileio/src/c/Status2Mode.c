/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
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
    return (100 * md + 10 * plus + bin);
}
/*--------------------------------------------------------------------------*/
