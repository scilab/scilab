
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
int C2F(genkron)(integer *typ,int *dx,integer *incx,integer *mx,integer *nx,int *dy,integer *incy,integer *my,integer *ny,int *dr,integer *incr);
#endif /* __GENKRON_H__ */
