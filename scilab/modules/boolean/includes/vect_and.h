/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
 * Copyright (C) 2012 - DIGITEO - Allan CORNET
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

#ifndef __VECT_AND_H__
#define __VECT_AND_H__

#include "dynlib_boolean.h"

#define AND_ON_ALL 0
#define AND_BY_ROWS 1
#define AND_BY_COLUMNS 2

/**
 * logical "and" of the elements of an array
 *
 * @param[in] v boolean matrix as input
 * @param[in] m rows size
 * @param[in] n columns size
 * @param[out] r result of logical "and"
 * @param[in] opt: AND_ON_ALL, AND_BY_ROWS or AND_BY_COLUMNS
 */

BOOLEAN_IMPEXP void vect_and(const int *v, int m, int n, int *r, int opt);

#endif /* __VECT_AND_H__ */
