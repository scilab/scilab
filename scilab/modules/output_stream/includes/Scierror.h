/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCIERROR__
#define __SCIERROR__

#include <stdarg.h>


/* 
* as sciprint but with an added first argument 
* which is ignored (used in do_printf) 
* @param iv error code
* @param fmt
* @param ...
*/
int  Scierror(int iv,char *fmt,...);

#endif /* __SCIERROR__ */
/*-----------------------------------------------------------------------------------*/ 
