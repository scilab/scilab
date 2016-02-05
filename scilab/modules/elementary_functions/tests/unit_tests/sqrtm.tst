// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

assert_checkalmostequal(sqrtm([8 8 ; 8 8]), [2 2 ; 2 2]);

x = [0 1; 2 4];
w = sqrtm(x);
assert_checkalmostequal(norm(w*w-x), 0, [], 3*%eps);
x(1, 2) = %i;
w = sqrtm(x);
assert_checkalmostequal(norm(w*w-x, 1), 0, [], 15*%eps);


// Error checks
refMsg = msprintf(_("%s: Wrong type for input argument #%d: A real or complex matrix expected.\n"),"sqrtm", 1);
assert_checkerror("sqrtm(list())", refMsg);
assert_checkerror("sqrtm(%s)", refMsg);
assert_checkerror("sqrtm(%t)", refMsg);

refMsg = msprintf(_("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"sqrtm", 1);
assert_checkerror("sqrtm([1 2 3; 4 5 6])", refMsg);
assert_checkerror("sqrtm([1 2; 3 4; 5 6])", refMsg);
