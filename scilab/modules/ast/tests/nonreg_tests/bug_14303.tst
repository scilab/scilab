// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aime AGNEL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14303 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14303
//
// <-- Short Description -->
// Display of matrices with scientific notation and lots of digits
// were wrong

A = [1,0,0,0,0,0,0,0;
0,1,0,0,0,0,0,0;
-1,0,2,0,0,0,0,0;
0,-3,0,4,0,0,0,0;
1,0,-8,0,8,0,0,0;
0,5,0,-20,0,16,0,0;
-1,0,18,0,-48,0,32,0;
0,-7,0,56,0,-112,0,64]
//display is ok

disp(nearfloat("succ", expm(A)));
// now display is ok

format(25);
disp(nearfloat("succ", expm(A)));

A =  matrix(linspace(-1E10,1E20,64), 8, 8);
disp(A);
format(10);
disp(A);

s = ["-0.034" "- 0.034"  "+1234.5678" "1234 5678"
"1e43"   "-3.5d-12" "-1.2+i"     "i+1.2"
""       "%inf"     "Inf"        "-Inf"
"%i"     "%pi"      "%e"         "%eps"
];
format(10);
disp(strtod(s));

// display with complex numbers
A =  matrix(linspace(-1E10,1E20,64), 8, 8);
B =  matrix(linspace(1E20,-1E10,64), 8, 8);
B(7, 7) = %nan;
B(2, 1) = %inf;
A(2, 1) = %nan;
A(3, 3) = %nan;
B(4, 4) = 0;
B(4, 3) = -B(4, 3);
A(4, 3) = -A(4, 3);
A(5, 5) = 0;
B(5, 5) = -B(5, 5);
A(6, 6) = 0;
B(6, 6) = 1;
format(25);
disp(A + %i * B);
format(10);
disp(A + %i * B);
format("e");
disp(A + %i * B);

