/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __WITHTK_H__
#define __WITHTK_H__

#include "machine.h" /* C2F , BOOL*/

/** 
* check if it is with TCL/TK
* @param rep 1 (WITH) or 0 (WITHOUT)
* @return 0
*/
int C2F(withtk)(int *rep);

/** 
* check if it is with TCL/TK
* @return TRUE or FALSE
*/
BOOL withtk(void);

#endif /* _WITHTK_H__ */
/*--------------------------------------------------------------------------*/ 
