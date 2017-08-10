
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
INTEGER_IMPEXP int C2F(genmsum)(int *typ, int *job, int *a, int *na, int *m, int *n, int *v, int *nv);
INTEGER_IMPEXP int C2F(genmsumasdouble)(int *typ, int *job, int *a, int *na, int *m, int *n, double *v, int *nv);

#endif /* __GENMSUM_H__ */

