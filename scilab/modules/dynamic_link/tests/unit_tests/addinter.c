#include <math.h>
#define __USE_DEPRECATED_STACK_FUNCTIONS__
#include "stack-c.h"

static double fun2(double x);

void fun1(double *x, double *y)
{
    *y = fun2(*x) / (*x);
}

static double fun2(double x)
{
    return( sin(x + 1));
}

int intfun1(char *fname)
{
    int m1, n1, l1;
    CheckRhs(1, 1);
    CheckLhs(1, 1);
    GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
    fun1(stk(l1), stk(l1));
    LhsVar(1) = 1;
    return 0;
}


