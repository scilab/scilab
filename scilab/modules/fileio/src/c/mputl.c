/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include <stdio.h>
#include "mputl.h"
#include "filesmanagement.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
#define MPUTL_FORMAT "%s\n"
/*--------------------------------------------------------------------------*/
mputlError mputl(int fileDescriptor, char **pStrings, int sizeStrings)
{
    FILE *fw = NULL;
    int i = 0;

    if (pStrings == NULL)
    {
        return MPUTL_ERROR;
    }

    if (fileDescriptor == STDIN_ID)
    {
        return MPUTL_INVALID_FILE_DESCRIPTOR;
    }
    else if (fileDescriptor == STDOUT_ID)
    {
        fw = stdout;
    }
    else
    {
        fw = GetFileOpenedInScilab(fileDescriptor);
        if (fw)
        {
            int mode = GetFileModeOpenedInScilab(fileDescriptor);

            if ( (mode >= 100) && (mode < 200) && ((mode % 100) < 10) /* not r+ */)
            {
                return MPUTL_NO_WRITE_RIGHT;
            }
        }
        else
        {
            return MPUTL_INVALID_FILE_DESCRIPTOR;
        }
    }

    for (i = 0; i < sizeStrings; i++)
    {
        if (fw == stdout)
        {
            sciprint(MPUTL_FORMAT, pStrings[i]);
        }
        else
        {
            fprintf(fw, MPUTL_FORMAT, pStrings[i]);
        }
    }

    return MPUTL_NO_ERROR;
}
/*--------------------------------------------------------------------------*/

