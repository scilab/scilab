/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __WITH_PVM_H__
#define __WITH_PVM_H__

#include "machine.h" /* C2F , BOOL*/

/** 
* check if it is with PVM
* @param rep 1 (WITH) or 0 (WITHOUT)
* @return 0
*/
int C2F(withpvm)(int *rep);

/** 
* check if it is with PVM
* @return TRUE or FALSE
*/
BOOL withpvm(void);

#endif /* __WITH_PVM_H__ */
/*--------------------------------------------------------------------------*/ 
