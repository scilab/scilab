
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
int C2F(genconcatcol)(integer *typ,integer *x1,integer *m1,integer *n1,integer *x2,integer *m2,integer *n2,integer *xr);

#endif /* __GENCONCATCOL_H__ */
