/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include "displ.h"
#include "stack-c.h"

int sci_view(char *fname)
{
    static int l1, m1, n1, l2, m2, n2, m3, n3, l3;
    static int minlhs = 1, maxlhs = 3, minrhs = 3, maxrhs = 3;

    /* Check number of inputs (rhs=3) and outputs (lhs=3) */
    CheckRhs(minrhs, maxrhs) ;
    CheckLhs(minlhs, maxlhs) ;

    /* Get X (1 ,double), Y (2, int) and  C (3, string) */
    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    GetRhsVar(2, MATRIX_OF_INTEGER_DATATYPE, &m2, &n2, &l2);
    GetRhsVar(3, STRING_DATATYPE, &m3, &n3, &l3);

    /* Call display function
       stk(l1)->X (double), istk(l2)->Y (int), cstk(l3)->Z  (char)    */
    displ(stk(l1), m1, n1, istk(l2), m2, n2, cstk(l3), m3, n3);

    /*  Return variables  */
    LhsVar(1) = 1;
    LhsVar(2) = 2;
    LhsVar(3) = 3;
    return 0;
}
