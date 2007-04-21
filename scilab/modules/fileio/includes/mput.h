/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#ifndef __MPUT_H__
#define __MPUT_H__

#include <stdio.h>
#include "machine.h"

/**
* mput functions  write data in a machine 
* independant way (i.e write in little-endian) 
* or can be used to write in little or big endian 
*/
void C2F(mput) (integer *fd, double *res, integer *n, char *type, integer *ierr);

void C2F(mputnc) (integer *fd, void * res, integer *n1, char *type, integer *ierr);

void mput2 (FILE *fa, integer swap2, double *res, integer n, char *type, integer *ierr);

#endif /* __MPUT_H__ */
/*-----------------------------------------------------------------------------------*/
