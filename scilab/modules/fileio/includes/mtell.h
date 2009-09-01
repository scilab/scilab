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
#ifndef __MTELL_H__
#define __MTELL_H__
#include "dynlib_fileio.h"
#include "machine.h"

/**
* tell function 
*/
FILEIO_IMPEXP void C2F(mtell) (int *fd, double *offset, int *err);

#endif /* __MTELL_H__ */
/*--------------------------------------------------------------------------*/

