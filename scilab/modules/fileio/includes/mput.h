/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
