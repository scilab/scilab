/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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

#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "lib/pipo.h"

int int_f4(char *fname)
{
    static int l1, m1, n1;
    CheckRhs(1, 1);
    CheckLhs(1, 1);
    GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1);
    CheckScalar(1, m1, n1);
    *istk(l1) = foo(*istk(l1));
    *istk(l1) = bar(*istk(l1));
    LhsVar(1) = 1;
    return 0;
}
