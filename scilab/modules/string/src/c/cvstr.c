
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Cong WU
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
#include "cvstr.h"
#include "core_math.h"
#include "getfastcode.h"
/*--------------------------------------------------------------------------*/
/* Table of constant values */
static int cx1 = 1;
static int c_n1 = -1;
/*--------------------------------------------------------------------------*/
int C2F(cvstr)(int * n, int * line, char * str, int * job, unsigned long str_len)
{
    if (*job == 0)
    {
        C2F(asciitocode)(n, line, str, &cx1, str_len);
    }
    else
    {
        C2F(codetoascii)(n, line, str, str_len);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(cvstr1)(int *n, int * line, char * str, int * job,	unsigned long  str_len)
{
    if (*job == 0)
    {
        C2F(asciitocode)(n, line, str, &c_n1, str_len);
    }
    else
    {
        C2F(codetoascii)(n, line, str, str_len);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(codetoascii)(int *n, int * line, char * str, unsigned long str_len)
{
    int j = 0;
    int nn = 0;

    /* check *n value */
    if  (*n >= 0)
    {
        nn = *n;
    }

    /* conversion code -> ascii */
    while (nn--)
    {
        str[j] = convertScilabCodeToAsciiCode(line[j]);
        j++;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(asciitocode)(int * n, int * line, char * str, int * flagx, unsigned long  str_len)
{
    int nn = 0;

    if (*flagx == 1)
    {
        int j = 0;
        nn = *n;
        while (nn--)
        {
            unsigned char current_char = str[j];
            line[j] = convertAsciiCodeToScilabCode(current_char);
            j++;
        }
    }
    else
    {
        int l = 0;

        /* check *n value */
        if ( (int)strlen(str) > *n )
        {
            l = (int)strlen(str);
        }
        else
        {
            l = *n;
        }

        for (nn = l - 1 ; nn >= 0; --nn)
        {
            unsigned char current_char = str[nn];
            line[nn] = convertAsciiCodeToScilabCode(current_char);
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
