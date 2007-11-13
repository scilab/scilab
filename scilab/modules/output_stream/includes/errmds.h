/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __ERRMDS_H__
#define __ERRMDS_H__

#include "machine.h"

/* routine used by fortran */

/**
* this routine extract error modes out of errct variable
* @param imess : if 0 error message is displayed
* @param imode : error recovery mode
* @param num  : error to catch, if num=-1 all errors are catched
* @return 0
*/
int C2F(errmds)(integer *num, integer *imess, integer *imode);

#endif /* __ERRMDS_H__ */
/*-----------------------------------------------------------------------------------*/
