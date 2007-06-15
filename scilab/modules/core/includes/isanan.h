/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __ISANAN_H__
#define __ISANAN_H__

#include "machine.h" /* C2F */

/**
* testing Nan returns 1 if a Nan is found and 0 elsewhere 
* @param value to check
* @return 1 if a Nan is found and 0 elsewhere
*/
integer C2F(isanan)(double *x);

#endif /* __ISANAN_H__ */
/*-----------------------------------------------------------------------------------*/ 
