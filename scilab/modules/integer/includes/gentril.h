
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GENTRIL_H__
#define __GENTRIL_H__
#include "machine.h"
#include "dynlib_integer.h"
#include "def.h"

/**
 * TODO : comment
 * @param typ
 * @param m
 * @param n
 * @param k
 * @param dx
 * @return
 */
INTEGER_IMPEXP int C2F(gentril)(int *typ, int *m, int *n, int *k, int *dx);
#endif /* __GENTRIL_H__ */
