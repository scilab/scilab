/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */


#include "../../routines/machine.h"

/*******************************************
 * External type for intersci
 * creating a new external type
 * ccalc12 : the wrapped function which returns a handler
 * to a dynamically allocated object
 * ccalc12f : function for the wrapper which <<converts>>
 *         Array to Scilab data
 *******************************************/

typedef struct array
{
    int m, n;
    double *val;
} Array ;

C2F(ccalc12) ( a, m, n, err)
Array **a;
int *m, *n, *err ;
{
    int i;
    *a = ( Array *) malloc( sizeof(Array));
    if ( *a == (Array *) 0)
    {
        *err = 1;
        sciprint("No more space\n");
    }
    *m = (*a)->m = 1;
    *n = (*a)->n = 10;
    (*a)->val = (double *)  malloc( (unsigned) (*m) * (*n) * sizeof(double));
    if ( (*a)->val  == (double *) 0 )
    {
        *err = 1;
        sciprint("No more space\n");
    }
    *err = 0;
    for ( i = 0 ; i < (*m) * (*n) ; i++)
    {
        (*a)->val[i] = i ;
    }
}


C2F(ccalc12f)(n, ip, op)
int *n;
Array **ip;
double *op;
{
    int i;
    if ( *n > 0 )
    {
        for (i = 0; i < *n; i++)
        {
            op[i] = (*ip)->val[i];
        }
        free((char *)(*ip)->val);
        free((char *)(*ip));
    }
}






