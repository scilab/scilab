/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 */

#include <math.h>
#include "stack-c.h"

/************************************
 *     simple example 4 (reading a scilab chain)
 *     -->link('ext4c.o','ext4c','C');
 *     -->a=[1,2,3];b=[4,5,6];n=3;YesOrNo='yes'
 *     -->c=call('ext4c',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d')
 *     -->c=sin(a)+cos(b)
 *     -->YesOrNo='no'
 *     -->c=a+b
 *     -->clear yes  --> undefined variable : yes
 ************************************/

#define MAXCH 10

int ext4c(int *n, double *a, double *b, double *c)
{
    int k;
    char ch[MAXCH];
    int lch = MAXCH;

    /* We search a Scilab Object name YesOrNo
     * check that it is a string
     * and store the string in ch
     * lch is used on entry to give the maximum number
     * of characters which can be stored in ch
     * After the call lch contains the number of
     * copied characters
     */

    ReadString("YesOrNo", &lch, ch);
    /******************************/
    if (strcmp(ch, "yes") == 0)
    {
        for (k = 0; k < *n; ++k)
        {
            c[k] = sin(a[k]) + cos(b[k]);
        }
    }
    else
    {
        for (k = 0; k < *n; ++k)
        {
            c[k] = a[k] + b[k];
        }
    }
    return (0);
}

