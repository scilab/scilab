/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SYNCEXEC_H__
#define __SYNCEXEC_H__
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"

/**
* execute a macro in scilab
* @param macro to execute (a string , "a=1+3;")
* @param length of macro (here 6)
* @param code error returned 
* @param mode sequential or not
* @param length of macro (again, fortran)
*/
int C2F(syncexec)(char *str, int *ns, int *ierr, int *seq, long int str_len);

#endif /* __SYNCEXEC_H__ */
/*-----------------------------------------------------------------------------------*/ 
