/*--------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#ifndef __MGETSTR_H__
#define __MGETSTR_H__

#include "machine.h"

/**
* read a string 
*/
void C2F(mgetstr) (integer *fd, char **start, integer *n, integer *ierr);
void C2F(mgetstr1) (integer *fd, char *start, integer *n, integer *ierr);

#endif /* __MGETSTR_H__ */
/*--------------------------------------------------------------------------*/
