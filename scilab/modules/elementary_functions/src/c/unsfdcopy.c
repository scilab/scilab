/*
 * Copyright (C) 1978-1993 - Jack Dongarra, linpack
 * Modified 12/3/93, array(1) declarations changed to array(*)
 * Copyright (C) 2007 - INRIA - Allan CORNET (translation to C)
 */

/*--------------------------------------------------------------------------*/
/* rewrite unsfdcopy.f */
/*--------------------------------------------------------------------------*/
/* WARNING :*/
/* ALWAYS BUILD unsfdcopy without optimization (Blended) */
/* unsfdcopy same thing as scidcopy but built without optimization */
/*--------------------------------------------------------------------------*/
#include <string.h> /* memcpy */
#include "machine.h"
#include "unsfdcopy.h"
/*--------------------------------------------------------------------------*/
int C2F(unsfdcopy)(int *n, long long *dx, int *incx, long long *dy, int *incy)
{
    if (*n <= 0)
    {
        return 0;
    }

    if ( (*incx == 1) && (*incy == 1) )
    {
        /*  code for both increments equal to 1 */
        /*  clean-up loop */
        memmove(dy , dx , (*n * sizeof(double)) );
    }
    else
    {
        int i = 0;

        /* code for unequal increments or equal increments */
        /* not equal to 1 */
        int ix = *incx >= 0 ? 0 : (1 - *n) * *incx;
        int iy = *incy >= 0 ? 0 : (1 - *n) * *incy ;

        for (i = 0; i < *n; i++)
        {
            dy[iy] = dx[ix];
            ix += *incx;
            iy += *incy;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
