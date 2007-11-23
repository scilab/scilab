/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __GENSUM_H__
#define __GENSUM_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param n
 * @param dx be carreful using this parameter (crappy way of doing polymorphism)
 * @param incx
 * @return 
 */
integer C2F(gensum)(integer *typ, integer *n, void *dx, integer *incx);

#endif /* __GENSUM_H__ */
