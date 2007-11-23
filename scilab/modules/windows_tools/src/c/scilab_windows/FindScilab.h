/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __FINDSCILAB_H__
#define __FINDSCILAB_H__

#include "machine.h" /* BOOL */

/**
* Check if exists another scilab window 
* @return TRUE or FALSE
*/
BOOL HaveAnotherWindowScilab(void);

/**
* open a dialog box to select another scilab
* @return a string
*/
char * ChooseAnotherWindowScilab(void);
#endif /*__FINDSCILAB_H__ */
/*--------------------------------------------------------------------------*/ 
