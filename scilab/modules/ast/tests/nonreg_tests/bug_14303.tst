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

disp(expm(A));
// now display is ok

format(25);
disp(expm(A));

A =  (rand(8,8) - 0.5) .* 10 .^ (100*rand(8,8));
disp(A);
format(10);
disp(A);

