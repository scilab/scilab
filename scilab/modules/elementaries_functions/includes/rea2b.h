/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __REA2B_H__
#define __REA2B_H__

/**
* translate a float vector to double precision vector
* @param n size of dx vector
* @param dx float vector 
* @param incx increment order
* @param dy double precision vector
* @param incy increment order
*/
int C2F(rea2db)(int *n, float *dx, int *incx, double *dy, int *incy);

#endif /* __REA2B_H__ */
/*-----------------------------------------------------------------------------------*/
