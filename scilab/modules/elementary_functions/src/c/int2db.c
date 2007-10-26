/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include "int2db.h"
/*-----------------------------------------------------------------------------------*/
int C2F(int2db)(int *n, int *dx, int *incx, double *dy, int *incy)
{
    int i1 = 0, i = 0;
    int ix = 0, iy = 0;

    --dy;
    --dx;
   
    if (*n <= 0) return 0;

    if (*incx == 1 && *incy == 1) 
	{
		/* code for both increments equal to 1 */
		i1 = *n;
		for (i = 1; i <= i1; ++i) dy[i] = (double) dx[i];
		return 0;
    }

	/* code for unequal increments or equal increments not equal to 1 */
    ix = 1;
    iy = 1;

    if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;
    if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;
	iy = (-(*n) + 1) * *incy + 1;
    i1 = *n;
    for (i = 1; i <= i1; ++i) 
	{
		dy[iy] = (double) dx[ix];
		ix += *incx;
		iy += *incy;
    }
    return 0;
} 
/*-----------------------------------------------------------------------------------*/

