
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
#ifndef __GENKRON_H__
#define __GENKRON_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param dx
 * @param incx
 * @param mx
 * @param nx
 * @param dy
 * @param incy
 * @param my
 * @param ny
 * @param dr
 * @param incr
 * @return 
 */
INTEGER_IMPEXP int C2F(genkron)(int *typ,int *dx,int *incx,int *mx,int *nx,int *dy,int *incy,int *my,int *ny,int *dr,int *incr);
#endif /* __GENKRON_H__ */
