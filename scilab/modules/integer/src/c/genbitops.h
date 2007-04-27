/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __GENBITOPS_H__
#define __GENBITOPS_H__

#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param op
 * @param n
 * @param dx
 * @param incx
 * @param dy
 * @param incy
 * @return 
 */
int C2F(genbitops)(integer *typ,integer *op,integer * n,int * dx,integer * incx,int * dy,integer * incy);

#endif /* __GENBITOPS_H__ */
