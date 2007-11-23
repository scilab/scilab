/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __XERHLT_H__
#define __XERHLT_H__

#include "machine.h" /* C2F */

/**
* do a long jump (slatec)
* @param messg a message 
* @param l lenght of messg
*/
void C2F(xerhlt) (char *messg, unsigned long l);

/**
* setjmp slatec
* @returns the value 0 if returning directly and  non-zero
* when returning from longjmp() using the saved context
*/
int setjmp_slatec_jmp_env(void);

#endif /* __XERHLT_H__ */
/*--------------------------------------------------------------------------*/ 
