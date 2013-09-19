/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "StringConvert.h"
/*--------------------------------------------------------------------------*/
int StringConvert(char *str)
/* changes `\``n` --> `\n` idem for \t and \r */
{
    char *str1;
    int count = 0;
    str1 = str;

    while ( *str != 0)
    {
        if ( *str == '\\' )
        {
            switch ( *(str + 1))
            {
                case 'n' :
                    *str1 = '\n' ;
                    str1++;
                    str += 2;
                    count++;
                    break;
                case 't' :
                    *str1 = '\t' ;
                    str1++;
                    str += 2;
                    break;
                case 'r' :
                    *str1 = '\r' ;
                    str1++;
                    str += 2;
                    break;
                default :
                    *str1 = *str;
                    str1++;
                    str++;
                    break;
            }
        }
        else
        {
            *str1 = *str;
            str1++;
            str++;
        }
    }
    *str1 = '\0';
    return count;
}
/*--------------------------------------------------------------------------*/
