/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __GENMMAX_H__
#define __GENMMAX_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param job
 * @param m
 * @param n
 * @param dx
 * @param incx
 * @param dy
 * @param dk
 * @param incy
 * @return 
 */
int C2F(genmmax)(integer *typ, integer *job, integer *m, integer *n, int *dx, integer *incx, int *dy, int *dk, integer *incy);

#endif /* __GENMMAX_H__ */

