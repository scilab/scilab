/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) DIGITEO - 2010 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "readline.h"
#include "mgetl.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
#define EMPTYSTR ""
/*--------------------------------------------------------------------------*/
int LineRead(int fd, char buf[], int n, int *cnt, int *nr)
{
    int returnedInfo = READNEXTLINE_ERROR_ERROR_UNMANAGED;
    int nbLinesToRead = 1;
    int nbLinesReaded = 0;
    int mgetIerr = MGETL_ERROR;

    wchar_t **lines = mgetl(fd, nbLinesToRead, &nbLinesReaded, &mgetIerr);
    char* line = wide_string_to_UTF8(lines[0]);
    freeArrayOfWideString(lines, nbLinesReaded);

    *cnt = 0;
    *nr = 0;

    memset(buf, 0, n);
    strcpy(buf, EMPTYSTR);

    switch (mgetIerr)
    {
        case MGETL_NO_ERROR:
        {
            if (line && nbLinesReaded == 1)
            {
                /* current limitation (bsiz) of line readed by scilab */
                if ((int)wcslen(lines[0]) < bsiz)
                {
                    strcpy(buf, line);
                    returnedInfo = READNEXTLINE_ERROR_EOL;
                }
                else
                {
                    strncpy(buf, line, bsiz);
                    returnedInfo = READNEXTLINE_ERROR_BUFFER_FULL;
                }
            }
            else
            {
                returnedInfo = READNEXTLINE_ERROR_EOF_REACHED;
            }
        }
        break;

        case MGETL_EOF:
        {
            if (lines)
            {
                if (nbLinesReaded == 0)
                {
                    returnedInfo = READNEXTLINE_ERROR_EOF_REACHED;
                }
                else
                {
                    /* current limitation (bsiz) of line readed by scilab */
                    if ((int)strlen(line) >= bsiz)
                    {
                        strcpy(buf, line);
                        returnedInfo = READNEXTLINE_ERROR_EOF_REACHED_AFTER_EOL;
                    }
                    else
                    {
                        strncpy(buf, line, bsiz);
                        returnedInfo = READNEXTLINE_ERROR_BUFFER_FULL;
                    }
                }
            }
            else
            {
                returnedInfo = READNEXTLINE_ERROR_EOF_REACHED_BEFORE_EOL;
            }
        }
        break;

        case MGETL_MEMORY_ALLOCATION_ERROR:
        case MGETL_ERROR:
        default:
        {
            returnedInfo = READNEXTLINE_ERROR_ERROR_UNMANAGED;
        }
        break;
    }

    *cnt = (int)strlen(buf) + 1;
    *nr = *cnt;

    if (line)
    {
        FREE(line);
    }

    return returnedInfo;
}
/*--------------------------------------------------------------------------*/
void C2F(readnextline)(int *fd, char buf[], int *n, int *count, int *nr, int *ierr)
{
    *ierr = LineRead(*fd, buf, *n, count, nr);
}
/*--------------------------------------------------------------------------*/
