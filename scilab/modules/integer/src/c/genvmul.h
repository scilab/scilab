/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __GENVMUL_H__
#define __GENVMUL_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param n
 * @param dx
 * @param incx
 * @param dy
 * @param incy
 * @return 
 */
int C2F(genvmul)(integer *typ,integer *n, integer1 *dx, integer *incx, integer1 *dy, integer *incy);

#endif /* __GENVMUL_H__ */
