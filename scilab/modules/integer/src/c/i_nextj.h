/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#ifndef __I_NEXTJ_H__
#define __I_NEXTJ_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param j
 * @param m
 * @param n
 * @param a
 * @param b
 * @return 
 */
int gengetcol(integer typ, integer j,integer m,integer n,integer *a,integer *b);
/**
 * TODO : comment
 * @param j
 * @return
 */
int C2F(inextj)(integer *j);

#endif /* __I_NEXTJ_H__ */
