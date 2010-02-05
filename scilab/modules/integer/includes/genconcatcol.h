
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDTU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GENCONCATCOL_H__
#define __GENCONCATCOL_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment
 * @param typ
 * @param x1
 * @param m1
 * @param n1
 * @param x2
 * @param m2
 * @param n2
 * @param xr
 * @return
 */
INTEGER_IMPEXP int C2F(genconcatcol)(int *typ,int *x1,int *m1,int *n1,int *x2,int *m2,int *n2,int *xr);

#endif /* __GENCONCATCOL_H__ */
