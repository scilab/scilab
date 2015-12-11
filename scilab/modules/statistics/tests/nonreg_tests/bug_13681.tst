//<-- CLI SHELL MODE -->
// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- Non-regression test for bug 13681 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13681
//
// <-- Short Description -->
// Calling cdfnor to get the "X" vector sometimes produced a vector with some NaN
// values.

Mean = 0;
Std = 1;
val = [9, 15, 30, 60, 100, 1000];
for i = 1:6
    p = linspace(1e-10,1-1e-10, val(i));
    q = 1-p;
    x = cdfnor("X", Mean*ones(p), Std*ones(p), p, q);
    assert_checkequal(find(isnan(x)), []);
end
