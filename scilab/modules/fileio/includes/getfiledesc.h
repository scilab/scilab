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
#ifndef __GETFILEDESC_H__
#define __GETFILEDESC_H__

#include "dynlib_fileio.h"
#include "machine.h"

/**
* returns the first available position in Files descriptor Table
* @param fd (id)
* @param[out] fd (-1 error)
*/
FILEIO_IMPEXP void C2F(getfiledesc)(int *fd); 

#endif /* __GETFILEDESC_H__ */
/*--------------------------------------------------------------------------*/
