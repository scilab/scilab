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
#ifndef __MOPEN_H__
#define __MOPEN_H__

#include "machine.h"

/**
* Opens file given by file and return it's id 
* @param[out] fd (id)
* @param filename
* @param status "r","w","a" or "rb","wb","ab"
* @param swap
* @param[out] res
* @param[out] error
*/

void C2F(mopen)(int *fd, char *file, char *status, int *f_swap, double *res, int *error);

#endif  /* __MOPEN_H__ */
/*--------------------------------------------------------------------------*/
