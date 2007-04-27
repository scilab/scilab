/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __DB2INT_H__
#define __DB2INT_H__

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
int C2F(db2int)(integer *typ, integer *n, double *dx, integer *incx, int *dy, integer *incy);

#endif /* __DB2INT_H__ */
