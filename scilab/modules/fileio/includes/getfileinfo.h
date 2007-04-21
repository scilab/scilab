/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#ifndef __GETFILEINFO_H__
#define __GETFILEINFO_H__

#include <stdio.h>
#include "machine.h"

/**
* get file info in the files table
*/
void C2F(getfileinfo)(integer *fd, FILE *fa, integer *swap, integer *type, integer *mode, char *filename, integer *lf, integer *ierr);

#endif /* __GETFILEINFO_H__ */

