/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/
#ifndef __GENKRON_H__
#define __GENKRON_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param dx
 * @param incx
 * @param mx
 * @param nx
 * @param dy
 * @param incy
 * @param my
 * @param ny
 * @param dr
 * @param incr
 * @return 
 */
int C2F(genkron)(integer *typ,int *dx,integer *incx,integer *mx,integer *nx,int *dy,integer *incy,integer *my,integer *ny,int *dr,integer *incr);
#endif /* __GENKRON_H__ */
