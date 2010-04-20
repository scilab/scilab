/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#if defined(__linux__)
#define _GNU_SOURCE /* Bug 5673 fix: avoid dependency on GLIBC_2.7 */
#endif

#include <string.h>
#include <stdio.h>
#include "NumTokens.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "BOOL.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
static BOOL checkLineHaveSeparator(char *line);
/*--------------------------------------------------------------------------*/
int NumTokens(char *string)
{
    if (string)
    {
        int n = 1;
        int lnchar = 0;
        int ntok   = -1;
        int length = (int)strlen(string)+1;

        if (string != 0)
        {
            /** Counting leading white spaces **/
            int r = sscanf(string, "%*[ \r\t\n]%n", &lnchar);
            if (!checkLineHaveSeparator(string))
            {
                return ntok;
            }
        }

        while ( n != 0 && n != EOF && lnchar <= length  )
        {
            char buf[128];
            int nchar1 = 0;
            int nchar2 = 0;
            char *strTmp = NULL;

            if (lnchar >= length)
            {
                return(ntok);
            }
            else
            {
                strTmp = strdup(&string[lnchar]);
            }

            ntok++;

            if (!checkLineHaveSeparator(strTmp))
            {
                if (strTmp)
                {
                    FREE(strTmp);
                    strTmp = NULL;
                }
                return ntok;
            }

            n = sscanf(strTmp, "%[^ \r\t\n]%n%*[ \r\t\n]%n", buf, &nchar1, &nchar2);
            lnchar += (nchar2 <= nchar1) ? nchar1 : nchar2;

            if (strTmp)
            {
                FREE(strTmp);
                strTmp = NULL;
            }
        }

        return(ntok);
    }
    return(1);
}
/*--------------------------------------------------------------------------*/
static BOOL checkLineHaveSeparator(char *line)
{
#define NUMBER_SEPARATOR_TYPE_1 ' '
#define NUMBER_SEPARATOR_TYPE_2 '\r'
#define NUMBER_SEPARATOR_TYPE_3 '\t'
#define NUMBER_SEPARATOR_TYPE_4 '\n'

    if ((strchr(line, NUMBER_SEPARATOR_TYPE_1) == NULL) &&
        (strchr(line, NUMBER_SEPARATOR_TYPE_2) == NULL) &&
        (strchr(line, NUMBER_SEPARATOR_TYPE_3) == NULL) &&
        (strchr(line, NUMBER_SEPARATOR_TYPE_4) == NULL))
    {
        return FALSE;
    }
    return TRUE;
}
/*--------------------------------------------------------------------------*/
