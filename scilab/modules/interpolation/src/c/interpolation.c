/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Bruno Pincon
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/
/*  PURPOSE
 *     interface code for some interpolation / approximation
 *     routines
 */
/*--------------------------------------------------------------------------*/
#include <math.h>
#include "interpolation.h"
/*--------------------------------------------------------------------------*/
int good_order(double x[], int n)
{
    /*  test if x[i-1] < x[i] */
    static int first = 1;
    int i;
    static double inf;

    if ( first )
    {
        inf = 1.0 / (double) (first - first) ;
        first = 0;
    }

    if (fabs(x[0]) == inf  ||  x[n - 1] == inf)
    {
        return ( 0 );
    }

    for ( i = 1 ; i < n ; i++ )
        if ( ! (x[i - 1] < x[i]) ) /* form of this test for detecting nan ... */
        {
            return ( 0 );
        }

    return ( 1 );
}
/*--------------------------------------------------------------------------*/







