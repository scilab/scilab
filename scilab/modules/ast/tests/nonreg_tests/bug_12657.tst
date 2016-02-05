// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 12657 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12657
//
// <-- Short Description -->
// Computation of v1.^v2 was not done in place,
// when v1 and v2 are real arrays, v1 >= 0 and v2 integer.

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

// Run with test_run('double', 'bug_12657', ['no_check_error_output'])

// Checking that the results for said cases are still ok
assert_checkequal([0.5 2; 100000 4.5578].^2, [0.25 4; 1e10 20.7735408400000025608]);
assert_checkequal([2.5].^[2 5; 10 5], [6.25 97.65625; 9536.7431640625 97.65625]);
assert_checkequal([0.5 2; 100000 4.5578].^[2 5; 5 2], [0.25 32; 1e25 20.7735408400000025608]);

// Check that IEEE compatibility is still assured
assert_checkequal([%inf %nan; %inf %nan].^%inf, [%inf %nan; %inf %nan]);
assert_checkequal([%inf].^[%inf %nan; %nan %inf], [%inf %nan; %nan %inf]);
assert_checkequal([%inf %nan; %nan %inf].^[%inf %nan; %inf %nan], [%inf %nan; %nan %nan]);

// Now, checking that computation is done in place
v = rand(1, 1e6);
v.^2;
// Checking the 1e5 first values of the result
assert_checktrue(and(sqrt(ans(1:1e5)) == v(1:1e5)));
// Getting random integer values
v = floor(10*v);
[2].^v;
// Checking the 1e5 first values of the result
res = ans(1:1e5);
v = v(1:1e5);
assert_checktrue(and(nthroot(res(find(v<>0)), v(find(v<>0))) == 2));
assert_checktrue(and(isnan(nthroot(res(find(v==0)), v(find(v==0))))));
