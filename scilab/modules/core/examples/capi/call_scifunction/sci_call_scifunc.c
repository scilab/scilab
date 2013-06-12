//-------------------------------------------------------------------------------------
/*
 * Scilab ( http://www.scilab.org/ )
 * Copyright (C) DIGITEO - Allan CORNET - 2009
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
//-------------------------------------------------------------------------------------
static int PutOneValueOnStack(double v);
//-------------------------------------------------------------------------------------
int sci_call_scifunc(char *fname)
{
    int m1 = 0, n1 = 0, l1 = 0;
    double v1 = 0.;

    int m2 = 0, n2 = 0, l2 = 0;
    double v2 = 0.;

    int m3 = 0, n3 = 0, l3 = 0;

    int rm1 = 0, rn1 = 0, rl1 = 0;
    double r = 0.;

    int positionFirstElementOnStackForScilabFunction = 0;
    int numberOfRhsOnScilabFunction = 0;
    int numberOfLhsOnScilabFunction = 0;
    int pointerOnScilabFunction = 0;

    CheckRhs(3, 3);
    CheckLhs(1, 1);

    if (GetType(1) != sci_matrix)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 1);
        return 0;
    }

    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    if ( (m1 == n1) && (n1 == 1) )
    {
        v1 = *stk(l1);
    }
    else
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 1);
        return 0;
    }


    if (GetType(2) != sci_matrix)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), fname, 2);
        return 0;
    }

    GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
    if ( (m2 == n2) && (n2 == 1) )
    {
        v2 = *stk(l2);
    }
    else
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, 2);
        return 0;
    }


    if (GetType(3) != sci_c_function)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scilab function expected.\n"), fname, 3);
        return 0;
    }

    // get pointer on external function (here scilabfoo)
    GetRhsVar(3, EXTERNAL_DATATYPE, &m3, &n3, &l3);

    // r = scilabfoo(x, y)
    // rhs eq. 2
    // lhs eq. 1

    // Position first element on Stack to use by Scilab Function
    // v = call_scifunc(300,120,scilabfoo);
    // On stack : 300 is on Top position (1)
    // 120 second position
    // scilabfoo third position
    // we want to pass 300 & 120 to scilab Function
    // First position is here : 1

    positionFirstElementOnStackForScilabFunction = 1;

    numberOfRhsOnScilabFunction = 2;

    numberOfLhsOnScilabFunction = 1;

    pointerOnScilabFunction = l3;

    // r = scilabfoo(x, y)
    // C2F(scifunction) call a scilab function
    C2F(scifunction)(&positionFirstElementOnStackForScilabFunction,
                     &pointerOnScilabFunction,
                     &numberOfLhsOnScilabFunction,
                     &numberOfRhsOnScilabFunction);

    // result r is now on first position on stack
    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &rm1, &rn1, &rl1);
    r = *stk(rl1);

    PutOneValueOnStack(r);

    return 0;
}
//-------------------------------------------------------------------------------------
static int PutOneValueOnStack(double v)
{
    int m = 1, n = 1, l = 0;

    CreateVar( Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &l );
    *stk(l) = v;
    LhsVar(1) = Rhs + 1;

    return 0;
}
//-------------------------------------------------------------------------------------
