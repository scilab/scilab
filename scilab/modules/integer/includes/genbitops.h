
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
#ifndef __GENBITOPS_H__
#define __GENBITOPS_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param op
 * @param n
 * @param dx
 * @param incx
 * @param dy
 * @param incy
 * @return 
 */
INTEGER_IMPEXP int C2F(genbitops)(int *typ,int *op,int * n,int * dx,int * incx,int * dy,int * incy);

#endif /* __GENBITOPS_H__ */
