/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __IDMIN_H__
#define __IDMIN_H__

#include "machine.h" /* C2F */

/**
 * finds the index of the first element having minimum value 
 * this function return 1 if x has only nan components : may be this is not a good behavior
 * this function doesn't test if n<1 or incx<1 : this is done by the scilab interface
 * @param n size of x
 * @param x vector
 * @param incr indice
 * @return 0
*/
int C2F(idmin)(int *n, double *x, int *incx);

#endif /* __IDMIN_H__ */
/*--------------------------------------------------------------------------*/ 
