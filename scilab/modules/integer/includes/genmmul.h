
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
#ifndef __GENMMUL_H__
#define __GENMMUL_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

/**
 * TODO : comment
 * @param typ
 * @param a
 * @param na
 * @param b
 * @param nb
 * @param c
 * @param nc
 * @param l
 * @param m
 * @param n
 * @return
 */
INTEGER_IMPEXP int C2F(genmmul)(int *typ, int *a, int *na, int *b, int *nb, int *c, int *nc, int *l, int *m, int *n);

#endif /* __GENMMUL_H__ */
