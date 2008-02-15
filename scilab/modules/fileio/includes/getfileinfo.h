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
#ifndef __GETFILEINFO_H__
#define __GETFILEINFO_H__

#include <stdio.h>
#include "machine.h"

/**
* get file info in the files table
*/
void C2F(getfileinfo)(integer *fd, FILE *fa, integer *swap, integer *type, integer *mode, char *filename, integer *lf, integer *ierr);

#endif /* __GETFILEINFO_H__ */

