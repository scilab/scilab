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
#include "sci_malloc.h"
#include "scistrtostr.h"
#include "cvstr.h"
/*--------------------------------------------------------------------------*/
#define MEM_LACK -3
/*--------------------------------------------------------------------------*/
int SciStrtoStr (int *Scistring, int *nstring, int *ptrstrings, char **strh)
{
    char *s = NULL, *p = NULL;
    int li = 0, ni = 0, *SciS = NULL, i = 0, job = 1;

    li = ptrstrings[0];
    ni = ptrstrings[*nstring] - li + *nstring + 1;

    p = (char *) MALLOC(ni * sizeof(char) );

    if (p == NULL)
    {
        return MEM_LACK;
    }

    SciS = Scistring;
    s = p;
    for ( i = 1 ; i < *nstring + 1 ; i++)
    {
        ni = ptrstrings[i] - li;
        li = ptrstrings[i];
        F2C(cvstr)(&ni, SciS, s, &job, (long int)ni);
        SciS += ni;
        s += ni;
        if (i < *nstring)
        {
            *s = '\n';
            s++;
        }
    }
    *s = '\0';
    *strh = p;
    return 0;
}
/*--------------------------------------------------------------------------*/
