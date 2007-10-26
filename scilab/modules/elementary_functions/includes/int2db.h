/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __INT2DB_H__
#define __INT2DB_H__

#include "machine.h" /* C2F */

/**
* translate a int vector to double precision vector
* @param n size of dx vector
* @param dx int vector 
* @param incx increment order
* @param dy double precision vector
* @param incy increment order
*/
int C2F(int2db)(int *n, int *dx, int *incx, double *dy, int *incy);

#endif /* __INT2DB_H__ */
/*-----------------------------------------------------------------------------------*/
