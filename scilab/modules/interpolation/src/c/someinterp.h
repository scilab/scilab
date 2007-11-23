/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __SOMEINTERP_H__
#define __SOMEINTERP_H__

/**
 * TODO : comment 
 * @param x
 * @param val
 * @param dim
 * @param n
 * @param xp
 * @param yp
 * @param np
 * @param outmode
 */
void nlinear_interp(double **x , double val[], int dim[], int n,
		    double **xp, double yp[], int np, int outmode, 
					double u[], double v[], int ad[], int k[]);

#endif /* __SOMEINTERP_H__ */
