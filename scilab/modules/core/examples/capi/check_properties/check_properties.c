#include <stack-c.h>
#include <sciprint.h>

int sci_check_properties_1(char * fname)
{
    int m1, n1, l1;
    int m2, n2, l2;
    int m3, n3, l3;
    int m4, n4, l4;
    int m5, n5, l5;

    CheckRhs(5, 5);
    CheckLhs(0, 1) ;

    ////////////////////////////
    // Getting first argument //
    ////////////////////////////

    GetRhsVar(1, "d", &m1, &n1, &l1);

    CheckVector(1, m1, n1); // Check that first argument is a vector
    CheckLength(1, m1 * n1, 4); // Check vector length

    /////////////////////////////
    // Getting second argument //
    /////////////////////////////

    GetRhsVar(2, "d", &m2, &n2, &l2);

    CheckRow(2, m2, n2); // Checks that second argument is a row vector
    // CheckColumn can also be used

    CheckDimProp(1, 2, m1 * n1 != n2); // Check compatibility beetween arg 1 and arg 2. We want m1*n1 == n2

    ////////////////////////////
    // Getting third argument //
    ////////////////////////////

    GetRhsVar(3, "d", &m3, &n3, &l3);

    CheckSameDims(1, 3, m1, n1, m3, n3); // Checks that arg 1 and arg3 have same dimensions

    /////////////////////////////
    // Getting fourth argument //
    /////////////////////////////

    GetRhsVar(4, "d", &m4, &n4, &l4);

    CheckScalar(4, m4, n4); // arg 4 must be scalar

    /////////////////////////////
    // Getting fourth argument //
    /////////////////////////////

    GetRhsVar(5, "d", &m5, &n5, &l5);

    CheckSquare(5, m5, n5); // square matrix
    CheckDims(5, m5, m5, 5, 5); // check dimensions

    LhsVar(1) = 0;

    return 0;
}

// We must be careful on the scilab name function (8 chars max).

int sci_check_properties_2(char * fname)
{
    int m1, n1, l1;

    CheckRhs(1, 1);
    CheckLhs(0, 1) ;

    switch (VarType(1))
    {
        case 1:
            GetRhsVar(1, "d", &m1, &n1, &l1);
            sciprint("1 is a scalar matrix\n");
            break;
        case 10:
            GetRhsVar(1, "c", &m1, &n1, &l1);
            sciprint("1 is a string\n");
            break;
        case 5:
            sciprint("1 is a sparse trying to overload\n");
            OverLoad(1);
    }

    LhsVar(1) = 0;

    return 0;
}
