
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
#ifndef __GENMMAX_H__
#define __GENMMAX_H__
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param job
 * @param m
 * @param n
 * @param dx
 * @param incx
 * @param dy
 * @param dk
 * @param incy
 * @return 
 */
int C2F(genmmax)(int *typ, int *job, int *m, int *n, int *dx, int *incx, int *dy, int *dk, int *incy);

#endif /* __GENMMAX_H__ */

