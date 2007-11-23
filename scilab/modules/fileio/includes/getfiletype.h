/*--------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#ifndef __GETFILETYPE_H__
#define __GETFILETYPE_H__

#include "machine.h"

/**
 * get file type (Fortran or C)
 *
 * @param fd    
 * @param type  
 * @param ierr  
 */
void C2F(getfiletype)(integer *fd, integer *type, integer *ierr);

#endif /* __GETFILETYPE_H__ */
/*--------------------------------------------------------------------------*/
