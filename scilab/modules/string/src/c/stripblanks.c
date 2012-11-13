
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : The algorithm of stripblanks                                    */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gw_string.h"
#include "MALLOC.h"
#include "stripblanks.h"
#include "freeArrayOfString.h"
/*------------------------------------------------------------------------*/
static char* substr(const char*str, unsigned start, unsigned end);
/*------------------------------------------------------------------------*/
#define BLANK_CHARACTER ' '
#define TAB_CHARACTER 9
/*------------------------------------------------------------------------*/
static char* substr(const char*str, unsigned start, unsigned end)
{
    unsigned n = end - start;
    char *stbuf = NULL;
    stbuf = (char*)MALLOC(sizeof(char) * (n + 1));

    if (stbuf)
    {
        if ( strcmp(str, "") != 0 )
        {
            strncpy(stbuf, str + start, n);   /*Put a part of str into stbuf*/
            stbuf[n] = 0;
        }
        else
        {
            strcpy(stbuf, "");
        }
    }
    return stbuf;
}
/*------------------------------------------------------------------------*/
void stripblanks(char **InputStrings, char **OutputStrings, int InputStringsDim, BOOL withTAB)
{
    int x = 0;
    char *replacedstr = NULL;

    for ( x = 0; x < InputStringsDim ; x++)   /*Search for all the matrix*/
    {
        int lengthInput_One = 0;
        int lengthInput_Two = 0;
        int i = 0;

        /* search character ' ' or TAB from end of the string */
        for ( i = (int)strlen(InputStrings[x]) - 1 ; i > 0 ; i--)
        {
            if (withTAB)
            {
                if ( (InputStrings[x][i] == (char)BLANK_CHARACTER) ||
                        (InputStrings[x][i] == (char)TAB_CHARACTER) )
                {
                    continue;
                }
                else
                {
                    lengthInput_Two = i;
                    break;
                }
            }
            else
            {
                if (InputStrings[x][i] == (char)BLANK_CHARACTER)
                {
                    continue;
                }
                else
                {
                    lengthInput_Two = i;
                    break;
                }
            }
        }
        /* search character ' ' or TAB from beginning of the string */
        if (lengthInput_Two > 0)
        {
            for (i = 0; i < (int)strlen(InputStrings[x]); i++)
            {
                if (withTAB)
                {
                    if ( (InputStrings[x][i] == (char)BLANK_CHARACTER) ||
                            (InputStrings[x][i] == (char)TAB_CHARACTER) )
                    {
                        continue;
                    }
                    else
                    {
                        lengthInput_One = i;
                        break;
                    }
                }
                else
                {
                    if (InputStrings[x][i] == (char)BLANK_CHARACTER)
                    {
                        continue;
                    }
                    else
                    {
                        lengthInput_One = i;
                        break;
                    }
                }
            }
        }

        if (lengthInput_One <= lengthInput_Two )
        {
            if ( (lengthInput_Two == 0)
                    && ( (InputStrings[x][0] == (char)BLANK_CHARACTER )
                         || (withTAB && (InputStrings[x][0] == (char)TAB_CHARACTER)) ) )
            {
                strcpy(OutputStrings[x], "\0");
            }
            else
            {
                /*Get the substring without tabs*/
                replacedstr = substr(InputStrings[x], lengthInput_One, lengthInput_Two + 1 );
                /*To add the substring into the output matrix*/
                strcpy(OutputStrings[x], replacedstr);
            }

            if (replacedstr)
            {
                FREE(replacedstr);
                replacedstr = NULL;
            }
        }
        else
        {
            strcpy(OutputStrings[x], InputStrings[x]);
        }
    }
}
/*------------------------------------------------------------------------*/
