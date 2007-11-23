/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#include "rea2db.h"
/*--------------------------------------------------------------------------*/
/* cette subroutine traduit un vecteur dx, de taille n, sur un
   vecteur double precision dy.
   dans le cas de deux increments egaux a 1, cette fonction
   emploie des boucles "epanouies".
   dans le cas ou les increments sont negatifs cette
   fonction prend les composantes en ordre inverse.
*/
/*--------------------------------------------------------------------------*/
int C2F(rea2db)(int *n, float *dx, int *incx, double *dy, int *incy)
{
	int i1 = 0, i = 0, ix = 0, iy = 0;

     /* Parameter adjustments */
    --dy;
    --dx;

    if (*n <= 0) return 0;

    if (*incx == 1 && *incy == 1)
	{
		/* code for both increments equal to 1 */
		i1 = *n;
		for (i = 1; i <= i1; ++i) dy[i] = (double)dx[i];
		return 0;
    }

	/* code for unequal increments or equal increments not equal to 1 */
    ix = 1;
    iy = 1;

    if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;
    if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;

    i1 = *n;
    for (i = 1; i <= i1; ++i)
	{
		dy[iy] = (double)dx[ix];
		ix += *incx;
		iy += *incy;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/

