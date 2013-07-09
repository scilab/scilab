//===================================================
// Allan CORNET - INRIA - 2008
// dummy test
//===================================================
#include <math.h>
#include "stack-c.h"
//===================================================
int interf_template_A0(char *fname)
{
    int n1 = 1, m1 = 1, l1 = 0;
    int iValue = 0;

    CheckRhs(0, 0);
    CheckLhs(1, 1);
    CreateVar(1, "i", &m1, &n1 , &l1);
    *istk(l1) = iValue;
    LhsVar(1) = Rhs + 1;
    return 0;
}
//===================================================
int interf_template_B0(char *fname)
{
    int n1 = 1, m1 = 1, l1 = 0;
    int iValue = 0;

    CheckRhs(0, 0);
    CheckLhs(1, 1);
    CreateVar(1, "i", &m1, &n1 , &l1);
    *istk(l1) = iValue;
    LhsVar(1) = Rhs + 1;
    return 0;
}
//===================================================

