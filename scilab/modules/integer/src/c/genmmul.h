/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __GENMMUL_H__
#define __GENMMUL_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param a
 * @param na
 * @param b
 * @param nb
 * @param c
 * @param nc
 * @param l
 * @param m
 * @param n
 * @return 
 */
int C2F(genmmul)(integer *typ, int *a, integer *na, int *b, integer *nb, int *c, integer *nc, integer *l, integer *m, integer *n);

#endif /* __GENMMUL_H__ */
