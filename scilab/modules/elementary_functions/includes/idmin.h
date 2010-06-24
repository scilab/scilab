/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#ifndef __IDMIN_H__
#define __IDMIN_H__

#include "machine.h" /* C2F */
#include "dynlib_elementary_functions.h"

/**
 * finds the index of the first element having minimum value 
 * this function return 1 if x has only nan components : may be this is not a good behavior
 * this function doesn't test if n<1 or incx<1 : this is done by the scilab interface
 * @param n size of x
 * @param x vector
 * @param incr indice
 * @return 0
*/
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(idmin)(int *n, double *x, int *incx);

#endif /* __IDMIN_H__ */
/*--------------------------------------------------------------------------*/ 
