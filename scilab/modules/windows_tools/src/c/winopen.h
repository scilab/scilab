/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __WINOPEN_H__
#define __WINOPEN_H__

#include "machine.h" /* BOOL */

/**
* Opens the item specified by the filename parameter. The item can be a file or folder
* @param[in] a filename. SCI is converted 
* @return TRUE or FALSE
*/
BOOL winopen(char *scilabfilename);

#endif /* __WINOPEN_H__ */
/*--------------------------------------------------------------------------*/
