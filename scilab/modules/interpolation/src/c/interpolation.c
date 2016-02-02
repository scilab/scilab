/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Bruno Pincon
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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







