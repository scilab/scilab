/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __GENDOT_H__
#define __GENDOT_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param n
 * @param dx be carreful using this parameter (crappy way of doing polymorphism)
 * @param incx
 * @param dy be carreful using this parameter (crappy way of doing polymorphism)
 * @param incy
 * @return 
 */
int C2F(gendot)(integer *typ,integer *n, void *dx, integer *incx, void *dy, integer *incy);
#endif /* __GENDOT_H__ */

