// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
// Copyright (C) 2013 - Michaël Baudin
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 9196 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9196
//
// <-- Short Description -->
//    The threshold level for conditioning in backslash is too small.
// =============================================================================

n=9;
A = testmatrix("hilb",n);
b=ones(n,1);
xexpected=[7129/2520
           4861/2520
           42131/27720
           35201/27720
           395243/360360
           348911/360360
           62575/72072
           113567/144144
           1768477/2450448];
x=A\b;
assert_checkalmostequal(x, xexpected, [], 1e-6);


Ac=complex(A,zeros(A));
bc=complex(b,zeros(b));
xc = Ac\bc;
xcexpected=complex(xexpected,zeros(xexpected));
assert_checkalmostequal(xc, xcexpected, [], 1e-5);

b=(1:n)';
xexpected=[9;
           17819/2520;
           82609/13860;
           47959/9240;
           415567/90090;
           299737/72072;
           45533/12012;
           71761/20592;
           988277/306306];
x=A\b;
assert_checkalmostequal(x, xexpected, [], 1e-5);
Ac=complex(A,zeros(A));
bc=complex(b,zeros(b));
xc = Ac\bc;
xcexpected=complex(xexpected,zeros(xexpected));
assert_checkalmostequal(xc, xcexpected, [], 1e-5);

b=[1;-1;1;-1;1;-1;1;-1;1];
xexpected=[1879/2520;
           893/2520;
           6557/27720;
           4993/27720;
           52901/360360;
           44911/360360;
           39173/360360;
           69659/720720;
           1068047/12252240];
x=A\b;
assert_checkalmostequal(x, xexpected, [], 1e-7);
Ac=complex(A,zeros(A));
bc=complex(b,zeros(b));
xc = Ac\bc;
xcexpected=complex(xexpected,zeros(xexpected));
assert_checkalmostequal(xc, xcexpected, [], 1e-6);

n=9;
A = testmatrix("hilb",n);
b = ones(n,1);
x = (b'/A')';
xexpected=[
2.82896825396825370e+000
1.92896825396825400e+000
1.51987734487734480e+000
1.26987734487734480e+000
1.09680042180042170e+000
9.68228993228993360e-001
8.68228993228993270e-001
7.87871850371850480e-001
7.21695379783615090e-001
];
assert_checkalmostequal(x, xexpected, [], 1e-6);
