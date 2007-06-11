/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#ifndef __MGET_H__
#define __MGET_H__

#include <stdio.h>
#include "machine.h"

/**
* mget functions  read data in a machine 
* independant way (i.e write in little-endian) 
* or can be used to read in little or big endian 
* if read fails *ierr contains the number of properly read items 
*/
void C2F(mget) (integer *fd, double *res, integer *n, char *type, integer *ierr);

void C2F(mgetnc)(integer *fd, void * res, integer *n1, char *type, integer *ierr);

void mget2(FILE *fa, integer swap, double *res, integer n, char *type, integer *ierr);

#endif /* __MGET_H__ */
/*-----------------------------------------------------------------------------------*/
