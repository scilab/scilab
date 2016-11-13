/*--------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
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
#include "sci_malloc.h"
#include "CreateEmptystr.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
char ** CreateEmptystr(int m1, int n1)
{
    int m1n1 = m1 * n1;
    char **OutputStrings = (char**)MALLOC(sizeof(char*) * (m1n1 + 1));

    if (OutputStrings)
    {
        int i = 0;
        for (i = 0; i < m1n1; i++) /*m1 is the number of row ; n1 is the number of col*/
        {
            OutputStrings[i] = (char*)MALLOC(sizeof(char) * (strlen(EMPTY_STRING) + 1));
            if (OutputStrings[i])
            {
                strcpy(OutputStrings[i], EMPTY_STRING);
            }
            else
            {
                freeArrayOfString(OutputStrings, m1n1);
                return NULL;
            }
        }
    }
    return OutputStrings;
}
/*--------------------------------------------------------------------------*/
