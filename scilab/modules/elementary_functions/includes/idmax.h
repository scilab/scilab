/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __IDMAX_H__
#define __IDMAX_H__

#include "machine.h" /* C2F */

/**
* finds the index of the first element having maximum value 
* @param n size of x
* @param x vector
* @param incr indice
* @return 0
*/
int C2F(idmax)(int *n, double *x, int *incx);

#endif /* __IDMAX_H__ */
/*-----------------------------------------------------------------------------------*/ 
