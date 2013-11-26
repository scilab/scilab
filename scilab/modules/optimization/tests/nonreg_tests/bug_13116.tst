// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
// Copyright (C) 2013 - Raise Partner - Mohamed Houacine
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13116 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13116
//
// <-- Short Description -->
// qpsolve did not respect the constraints

me = 1;
// Constraints: ci <= x <= cs
ci = [-0.5; -1; -1; -1; -1; -1; -1; -1; -1; -1];
cs = [0.5; 1; 1; 1; 1; 1; 1; 1; 1; 1];
b = 1;
C = ones(1, 10);
p = zeros(10, 1);
Q = [1D-07 0 0 0 0 0 0 0 0 0;
0  0.0000284  0.0000103  0.0000241  0.0000047 -0.0000031 -0.0000008  0.0000220  0.0000207  0.0000024;
0  0.0000103  0.0000336  0.0000238  0.0000088 -0.0000022 -0.0000004  0.0000093  0.0000114  0.0000034;
0  0.0000241  0.0000238  0.0000436  0.0000137 -0.0000005 -0.0000043  0.0000198  0.0000126  0.0000004;
0  0.0000047  0.0000088  0.0000137  0.0000330  0.0000041  0.0000017  0.0000077  0.0000055  0.0000109;
0 -0.0000031 -0.0000022 -0.0000005  0.0000041  0.0000269  0.0000003 -0.0000036 -0.0000054  0.0000094;
0 -0.0000008 -0.0000004 -0.0000043  0.0000017  0.0000003  0.0000092  0.0000033  0.0000005  0.0000028;
0  0.0000220  0.0000093  0.0000198  0.0000077 -0.0000036  0.0000033  0.0000497  0.0000218  0.0000018;
0  0.0000207  0.0000114  0.0000126  0.0000055 -0.0000054  0.0000005  0.0000218  0.0000343  0.0000033;
0  0.0000024  0.0000034  0.0000004  0.0000109  0.0000094  0.0000028  0.0000018  0.0000033  0.0000160 ];

[x, iact, iter, f] = qpsolve(Q, p, C, b, ci, cs, me);

assert_checktrue(abs(x(1))   <= 0.5);
assert_checktrue(abs(x(2:$)) <= 1);

// Example from the help page
//Find x in R^6 such that:
//C1*x = b1 (3 equality constraints i.e me=3)
C1 = [1 -1 1 0 3 1;
-1 0 -3 -4 5 6;
2 5 3 0 1 0];
b1 = [1; 2; 3];

//C2*x <= b2 (2 inequality constraints)
C2 = [0 1 0 1 2 -1;
-1 0 2 1 1 0];
b2 =[-1; 2.5];

//with  x between ci and cs:
ci = [-1000; -10000; 0; -1000; -1000; -1000];
cs = [10000; 100; 1.5; 100; 100; 1000];

//and minimize 0.5*x'*Q*x + p'*x with
p = [1; 2; 3; 4; 5; 6]; Q = eye(6, 6);

//No initial point is given;
C = [C1; C2];
b = [b1; b2];
me = 3;
[x, iact, iter, f] = qpsolve(Q, p, C, b, ci, cs, me);
//Only linear constraints (1 to 4) are active

assert_checktrue(x(1) >= -1000  & x(1) <= 10000);
assert_checktrue(x(2) >= -10000 & x(2) <= 100);
assert_checktrue(x(3) >=  0     & x(3) <= 1.5);
assert_checktrue(x(4) >= -1000  & x(4) <= 100);
assert_checktrue(x(5) >= -1000  & x(5) <= 100);
assert_checktrue(x(6) >= -1000  & x(6) <= 1000);
