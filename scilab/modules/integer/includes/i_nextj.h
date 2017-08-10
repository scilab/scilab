
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
#ifndef __I_NEXTJ_H__
#define __I_NEXTJ_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment
 * @param typ
 * @param j
 * @param m
 * @param n
 * @param a
 * @param b
 * @return
 */
INTEGER_IMPEXP int gengetcol(int typ, int j, int m, int n, int *a, int *b);
/**
 * TODO : comment
 * @param j
 * @return
 */
INTEGER_IMPEXP int C2F(inextj)(int *j);

#endif /* __I_NEXTJ_H__ */
