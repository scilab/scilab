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
#ifndef __MCLOSE_H__
#define __MCLOSE_H__

#include "machine.h"

/**
* close the file with id *fd 
* if *id != -1 and *id != -2
* the current file if *id = -1 
* all opened file if *id = -2 
*/
void C2F(mclose) (int *fd, double *res);

#endif /* __MCLOSE_H__ */
/*--------------------------------------------------------------------------*/
