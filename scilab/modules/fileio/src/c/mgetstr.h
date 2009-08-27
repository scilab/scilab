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
#ifndef __MGETSTR_H__
#define __MGETSTR_H__

#include "machine.h"

/**
 * read a string 
 *
 * @param fd    
 * @param start 
 * @param n 
 * @param ierr  
 */
void C2F(mgetstr1) (int *fd, char *start, int *n, int *ierr);

#endif /* __MGETSTR_H__ */
/*--------------------------------------------------------------------------*/
