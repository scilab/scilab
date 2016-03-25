// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for correl function 
// =============================================================================

assert_checkfalse(execstr("correl()"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),"correl",2,3);
assert_checkerror("correl()", refMsg);

assert_checkfalse(execstr("correl(1, [1 2])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Vector expected.\n"),"correl",1);
assert_checkerror("correl(1, [1 2])", refMsg);

assert_checkfalse(execstr("correl(""e"", [1 2])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Vector expected.\n"),"correl",1);
assert_checkerror("correl(""e"", [1 2])", refMsg);

assert_checkfalse(execstr("correl([1 2], 1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Vector expected.\n"),"correl",2);
assert_checkerror("correl([1 2], 1)", refMsg);

assert_checkfalse(execstr("correl([1 2], ""e"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Vector expected.\n"),"correl",2);
assert_checkerror("correl([1 2], ""e"")", refMsg);


r = correl([1 2 3], [8 9 1]);
assert_checkalmostequal(r, -0.802955068, [], %eps);

r = correl([1 2 3], [8 9 1], eye(3,3));
assert_checkalmostequal(r, -0.802955068, [], %eps);

r = correl([9 5 1 3 5 7], [8 4 2 6 0 8]);
assert_checkalmostequal(r, 0.6062178, [], 1.e-8);

r = correl([9 5 1 3 5 7], [8 4 2 6 0 8], eye(6,6));
assert_checkalmostequal(r, 0.6062178, [], 1.e-8);

r = correl([9 7 4 10 3 4 4 3 6], [5 4 6 6 5 3 7 5 10]);
assert_checkalmostequal(r, 0.0880254, [], 1.e-7);

r = correl([9 7 4 10 3 4 4 3 6]', [5 4 6 6 5 3 7 5 10]);
assert_checkalmostequal(r, 0.0880254, [], 1.e-7);

r = correl([9 7 4 10 3 4 4 3 6], [5 4 6 6 5 3 7 5 10]');
assert_checkalmostequal(r, 0.0880254, [], 1.e-7);

r = correl([9 7 4 10 3 4 4 3 6]', [5 4 6 6 5 3 7 5 10]');
assert_checkalmostequal(r, 0.0880254, [], 1.e-7);

r = correl([9 7 4 10 3 4 4 3 6], [5 4 6 6 5 3 7 5 10], eye(9,9));
assert_checkalmostequal(r, 0.0880254, [], 1.e-7);

assert_checkfalse(execstr("correl([1 4 9 7 2], [6 3 0])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Incompatible input arguments #%d and #%d: Same sizes expected"), "correl", 1, 2);
assert_checkerror("correl([1 4 9 7 2], [6 3 0])", refMsg);

r = correl([1 4 9 7 2], [6 3 0], eye(5,3));
assert_checkalmostequal(r, -0.9897433, [], 1.e-8);

r = correl([1 4 9 7 2], [6 3 0], [0.3 0.5 0.7;0.1 1 0.4;0.3 0.6 0.4;0.2 0.2 0.3;0.8 0.4 0.5]);
assert_checkalmostequal(r, -0.0089969, [], 1.e-7);

r = correl([1 4 9 7 2], [2 5 10 8 3])
assert_checkalmostequal(r, 1, 2*%eps);

r = correl([1 4 9 7 2], [-0 -3 -8 -6 -1])
assert_checkalmostequal(r, -1, 2*%eps);

x = [24.2; 22.7;30.5;28.6;25.5;32.0;28.6;26.5;25.3;26.0;24.4;24.8;20.6;25.1;21.4;23.7;23.9;25.2;27.4;28.3;28.8;26.6];
y = [25;31;36;33;19;24;27;25;16;14;22;23;20;25;25;23;27;30;33;32;35;24];
r = correl(x,y);
assert_checkalmostequal(r,0.4189440, [], 1.e-7);
