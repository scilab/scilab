// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13208 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13208
//
// <-- Short Description -->
// reglin returned NaNs when y contained NaNs.
// =============================================================================

// In the following case, we can graphically see that a1 and b1 should be equal
// to a2 and b2, because both problems represent two straight lines:
// one goes from from (0,0) to (10,10) and the other one goes from (0,20) to (10,30).
x = 0:10;
y = 20:30;
[a1, b1] = reglin(x, [x ; y]);

y(2:10) = %nan; // Leaving y(1) and y(11) unchanged.
[a2, b2] = nanreglin(x, [x ; y]);
assert_checkequal([a1 b1], [a2 b2]);


// Now both problems represent one straight line (reglin(x, x)) from (0,0) to (2,2),
// but while the second argument of the first problem (reglin(x, y)) represents
// a flat line (with equation y = 1), the second argument of the second problem
// (reglin(x, y2)) ignores the central point of y (set to %nan) so the flat line
// now has equation y = 0, because the two remaining points are (0,0) and (2,0).
x = 0:2;
y = [0 3 0];
[a, b] = reglin(x, [x ; y]);
assert_checkequal(a, [1 ; 0]);
assert_checkequal(b, [0 ; 1]);

y2 = y;
y2(2) = %nan; // y2 = [0 %nan 0];
[a, b] = nanreglin(x, [x ; y2]);
assert_checkequal(a, [1 ; 0]);
assert_checkequal(b, [0 ; 0]);


// Error checks
x = 1:3;
y = 1:3;
refMsg = msprintf(_("%s: No NaNs detected, please use %s() instead.\n"), "nanreglin", "reglin");
assert_checkerror("[a, b] = nanreglin(x, [x ; y])", refMsg);

y = [1 %nan 3];
refMsg = msprintf(_("%s: NaNs detected, please use %s() instead.\n"), "reglin", "nanreglin");
assert_checkerror("[a, b, sig] = reglin(x, [x ; y])", refMsg);

