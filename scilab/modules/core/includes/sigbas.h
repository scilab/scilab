/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __SIGBAS_H__
#define __SIGBAS_H__ 

#include "machine.h"

/**
* dealing with signals inside Scilab 
* @param[in] sig : a number signal
* @return 0
* sig == 2 ???
* sig == 8 Floating point exception
* sig == 11 fatal error!!!
*/
int C2F(sigbas)(integer *sig);

#endif /* __SIGBAS_H__  */
/*--------------------------------------------------------------------------*/
