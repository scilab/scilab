/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */
#include "matmul.h"

#include "stack-c.h"

#include "sciprint.h"

int sci_matmul(char *fname)
{
    static int l1, m1, n1, l2, m2, n2, l3;
    static int minlhs = 1, maxlhs = 1, minrhs = 2, maxrhs = 2;

    /* Check number of inputs (rhs=2) and outputs (lhs=1) */
    CheckRhs(minrhs, maxrhs) ;
    CheckLhs(minlhs, maxlhs) ;

    /* Get A (#1) and B (#2) and create C (#3) as double ("d") matrices */
    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);  /* m1, n1 (and l1) are output parameters */
    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);  /* m1, n1 (and l1) are output parameters */
    CreateVar(3, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n2, &l3);  /* m1 and n2 are input parameters */

    /* Check dimensions    */
    if (!(n1 == m2))
    {
        sciprint("%s: Incompatible inputs\n", "matmul");
        SciError(999);
        return 0;
    }

    /* Call multiplication function
     * inputs:stk(l1)->A, stk(l2)->B
     *  output:stk(l3)->C
     */
    matmul(stk(l1), m1, n1, stk(l2), n2, stk(l3));

    /*  Return C (3)  */
    LhsVar(1) = 3;
    return 0;
}
