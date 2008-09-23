
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __MPUTI_H__
#define __MPUTI_H__

#include "machine.h"

/**
 * TODO: add comment
 *
 * @param fd    
 * @param res   
 * @param n 
 * @param char type[]   
 * @param ierr  
 */
void C2F(mputi) (int *fd,int *res,int *n,char type[],int *ierr);

#endif /* __MPUTI_H__ */

