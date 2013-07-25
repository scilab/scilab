// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for legendre function 
// =============================================================================

// Interface
assert_checkfalse(execstr("legendre()"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "legendre", 3, 4);
assert_checkerror("legendre()", refMsg);

assert_checkfalse(execstr("legendre([1 2], [1 2], 1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Only one of arg1 and arg2 may be a vector.\n"), "legendre");
assert_checkerror("legendre([1 2], [1 2], 1)", refMsg);

assert_checkfalse(execstr("legendre(""r"", 1, [0.9 0.8])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Integer or vector of integers expected.\n"), "legendre", 1);
assert_checkerror("legendre(""r"", 1, [0.9 0.8])", refMsg);

assert_checkfalse(execstr("legendre(1, ""r"", [0.9 0.8])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Integer or vector of integers expected.\n"), "legendre", 2);
assert_checkerror("legendre(1, ""r"", [0.9 0.8])", refMsg);

assert_checkfalse(execstr("legendre(1, 1, ""r"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "legendre", 3);
assert_checkerror("legendre(1, 1, ""r"")", refMsg);

assert_checkfalse(execstr("legendre(1, 1, %i)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "legendre", 3);
assert_checkerror("legendre(1, 1, %i)", refMsg);

res = legendre(0, 0, [-1 -0.5 0 0.5 1]);
assert_checkequal(res, [1 1 1 1 1]);

res = legendre(0:2, 0, [-1 -0.5 0 0.5 1]);
expected = [1 1 1 1 1;
            -1 -0.5 0 0.5 1;
            1 -0.125 -0.5 -0.125 1];
assert_checkequal(res, expected);

res = legendre(2, 0:2, [-1 -0.5 0 0.5 1]);
expected = [1 -0.125 -0.5 -0.125 1;
            0 1.2990381 0 -1.2990381 0;
            0 2.25 3 2.25 0];
assert_checkalmostequal(res, expected, [], %eps);

res = legendre(2, 0:2, [0 0.5 1 0.9]);
expected = [-0.5 -0.125 1 0.715;
            0 -1.2990381 0 -1.1769027;
            3 2.25 0 0.57];
assert_checkalmostequal(res, expected, [], %eps);

res = legendre(2, 0:2, [0 -1 -0.2 -0.6]);
expected = [-0.5 1 -0.44 0.04;
            0 0 0.58787754 1.44;
            3 0 2.88 1.92];
assert_checkalmostequal(res, expected, [], %eps);

res = legendre(2, 0:2, [0 -1 -0.2 -0.6]');
expected = [-0.5 1 -0.44 0.04;
            0 0 0.58787754 1.44;
            3 0 2.88 1.92];
assert_checkalmostequal(res, expected, [], %eps);

res = legendre(2, 0:2, [0 -1 -0.2 -0.6], "norm");
expected = [-0.7905694 1.5811388 -0.6957011 0.0632456;
            0 0 -0.3794733 -0.929516;
            0.9682458 0 0.929516 0.6196773];
assert_checkalmostequal(res, expected, [], 1.e-7);

res = legendre(2, 0:2, [0 -1 -0.2 -0.6]', "norm");
expected = [-0.7905694 1.5811388 -0.6957011 0.0632456;
            0 0 -0.3794733 -0.929516;
            0.9682458 0 0.929516 0.6196773];
assert_checkalmostequal(res, expected, [], 1.e-7);

res = legendre(4, 2:4, [0 -0.2 0.2;1 0.7 -1]);
expected = [-7.5 0 -5.184 9.29475 -5.184 0;
            0 0 19.752685 -26.769644 -19.752685 0;
            105 0 96.768 27.3105 96.768 0];
assert_checkalmostequal(res, expected, [], 1.e-7);

res = legendre(4, 2:4, [0 -0.2 0.2;1 0.7 -1], "norm");
expected = [-0.8385255 0 -0.5795888 1.0391846 -0.5795888 0;
            0 0 -0.5902244 0.7998961 0.5902244 0;
            1.109265 0 1.0222986 0.2885198 1.0222986 0];
assert_checkalmostequal(res, expected, [], 1.e-7);
