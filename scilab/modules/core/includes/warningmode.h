/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __WARNINGMODE_H__
#define __WARNINGMODE_H__

#include "machine.h"

BOOL setWarningMode(BOOL ON);
BOOL getWarningMode(void);
int C2F(iswarningon)(int *rep); /* fortran version of getWarningMode */

#endif /*  __WARNINGMODE_H__ */
/*--------------------------------------------------------------------------*/
