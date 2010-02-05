
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
#ifndef __GENCHECK_H__
#define __GENCHECK_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment
 * @param typ
 * @param op
 * @param n
 * @param a
 * @param ia
 * @param b
 * @param ib
 * @param r
 * @return
 */

INTEGER_IMPEXP int C2F(gencheck)(int *typ, int *op, int *n, int *a, int *ia, int *b, int *ib, int *r);

#endif /* __GENCHECK_H__ */
