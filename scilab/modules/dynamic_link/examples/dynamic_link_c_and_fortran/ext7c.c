/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 */

#include "stack-c.h"

/******************************************
 *     example 7
 *     creating vector c in scilab internal stack
 *     -->link('ext7c.o','ext7c','C')
 *     -->a=[1,2,3]; b=[2,3,4];
 *     c does not exist (c made by the call to matz)
 *     -->call('ext7c',a,1,'d',b,2,'d','out',1);
 *     c now exists
 *     -->c=a+2*b
 ******************************************/


int ext7c(double *a, double *b)
{
    static int c1 = 1;
    static int c3 = 3;
    static char str[] = "test";
    int strl;

    double c[3];
    int k = 0;
    for (k = 0; k < 3; k++)
    {
        c[k] = a[k] + b[k] * 2.;
    }
    /** sending c[] to scilab variable c (size [1,3]) **/
    WriteMatrix("c", &c1, &c3, c);
    /** sending f characters of "test" to Scilab variable d **/
    strl = strlen(str);
    /* Creating the Scilab variable d from str */
    WriteString("d", &strl, str);
    return (0);
}

