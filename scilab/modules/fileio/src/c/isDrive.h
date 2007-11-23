/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __ISDRIVE_H__
#define __ISDRIVE_H__

#include "machine.h" /* BOOL */

/**
* check if strname is a drive (only for windows)
* @param[in] a string 
* @return TRUE or FALSE
* returns always false on linux
*/
BOOL isDrive(const char *strname);

#endif
/*--------------------------------------------------------------------------*/ 
