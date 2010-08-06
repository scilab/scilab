
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
#ifndef __GENMSUM_H__
#define __GENMSUM_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment 
 * @param typ
 * @param job
 * @param a
 * @param a
 * @param na
 * @param m
 * @param n
 * @param v
 * @param nv
 * @return 
 */
INTEGER_IMPEXP int C2F(genmsum)(int *typ,int *job, int *a, int *na, int *m, int *n, int *v, int *nv);
INTEGER_IMPEXP int C2F(genmsumasdouble)(int *typ,int *job, int *a, int *na, int *m, int *n, double *v, int *nv);

#endif /* __GENMSUM_H__ */

