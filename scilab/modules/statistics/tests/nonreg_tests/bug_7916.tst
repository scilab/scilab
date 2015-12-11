// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 7916 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7916
//
// <-- Short Description -->
//    nansum([]) returned NaN value.
// =============================================================================

assert_checkequal(sum([]), 0);
assert_checkequal(nansum([]), 0);
assert_checkequal(nansum(%nan), 0);
assert_checkequal(nansum([%nan %nan; 1 2]), 3);
assert_checkequal(nansum([%nan %nan; 1 2], "*"), 3);
assert_checkequal(nansum([%nan %nan; 1 2], "r"), [1 2]);
assert_checkequal(nansum([%nan %nan; 1 2], "c"), [0; 3]);
assert_checkequal(nansum([%nan %nan; 1 2], 1), [1 2]);
assert_checkequal(nansum([%nan %nan; 1 2], 2), [0; 3]);
