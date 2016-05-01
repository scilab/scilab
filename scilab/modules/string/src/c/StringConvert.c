/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009
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
int StringConvertW(wchar_t *str)
/* changes `\``n` --> `\n` idem for \t and \r */
{
    wchar_t *str1;
    int count = 0;
    str1 = str;

    while ( *str != 0)
    {
        if ( *str == L'\\' )
        {
            switch ( *(str + 1))
            {
                case L'n' :
                    *str1 = L'\n' ;
                    str1++;
                    str += 2;
                    count++;
                    break;
                case L't' :
                    *str1 = L'\t' ;
                    str1++;
                    str += 2;
                    break;
                case L'r' :
                    *str1 = L'\r' ;
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
    *str1 = L'\0';
    return count;
}
/*--------------------------------------------------------------------------*/
