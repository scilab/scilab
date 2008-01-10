/*----------------------------------------------------------------------*/
/* @author INRIA (initial fortran version)                              */
/* @author Bruno Pincon (2003) bug fix for the nan problem              */
/* @author Allan Cornet (2007) rewrite to C                             */
/*----------------------------------------------------------------------*/
#include "idmin.h"
#include "isanan.h"
/*--------------------------------------------------------------------------*/ 
int C2F(idmin)(int *n, double *x, int *incx)
{

    int x_dim1 = 0, x_offset  = 0, ret_val = 0, i1 = 0;
    int i = 0, j = 0;
    double xmin  = 0;
    
    x_dim1 = *incx;
    x_offset = 1 + x_dim1;
    x -= x_offset;

    ret_val = 1;
	/* initialize the min with the first component being not a nan */
    j = 1;
    while(C2F(isanan)(&x[j * x_dim1 + 1]) == 1) 
	{
		++j;
		if (j > *n) return ret_val;
    }
    xmin = x[j * x_dim1 + 1];
    ret_val = j;

	/* the usual loop */
    i1 = *n;
    for (i = j + 1; i <= i1; ++i) 
	{
		if (x[i * x_dim1 + 1] < xmin) 
		{
			/* a test with a nan must always return false */
			xmin = x[i * x_dim1 + 1];
			ret_val = i;
		}
    }
    return ret_val;
} 
/*--------------------------------------------------------------------------*/ 
