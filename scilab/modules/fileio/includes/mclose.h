/*--------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#ifndef __MCLOSE_H__
#define __MCLOSE_H__

#include "machine.h"

/**
* close the file with id *fd 
* if *id != -1 and *id != -2
* the current file if *id = -1 
* all opened file if *id = -2 
*/
void C2F(mclose) (integer *fd, double *res);

#endif /* __MCLOSE_H__ */
/*--------------------------------------------------------------------------*/
