/*--------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __ERROR_INTERNAL_H__
#define __ERROR_INTERNAL_H__

#define ERROR_FROM_FORTRAN 0
#define ERROR_FROM_C 1

#include "machine.h"

/**
* error_internal
* @param [in] n : error code
* @param [in] buffer : string error
* @param [in] mode : ERROR_FROM_FORTRAN (not use buffer) or ERROR_FROM_C
* @return 0
*/
int error_internal(integer *n,char *buffer,int mode);

#endif /* __ERROR_INTERNAL_H__ */
/*--------------------------------------------------------------------------*/ 
