
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GENCONCATROW_H__
#define __GENCONCATROW_H__
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
 * @return
 */
INTEGER_IMPEXP int C2F(genconcatrow)(int *typ, int *x1, int *m1, int *n1, int *x2, int *m2, int *n2);

#endif /* __GENCONCATROW_H__ */
