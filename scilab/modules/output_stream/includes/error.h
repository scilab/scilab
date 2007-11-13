/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __ERROR_H__
#define __ERROR_H__

#include "machine.h" /* C2F */

/* routine used by fortran */

/**
* error display and handling
* @param[in]  n : error number 
* @return 0
*/
int C2F(error)(integer *n);

#endif /* __ERROR_H__ */
/*-----------------------------------------------------------------------------------*/ 
