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
#ifndef __GETFILETYPE_H__
#define __GETFILETYPE_H__

#include "machine.h"

/**
 * get file type (Fortran or C)
 *
 * @param fd    
 * @param type  
 * @param ierr  
 */
void C2F(getfiletype)(int *fd, int *type, int *ierr);

#endif /* __GETFILETYPE_H__ */
/*--------------------------------------------------------------------------*/
