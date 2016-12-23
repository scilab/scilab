// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14648 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14648
//
// <-- Short Description -->
//  isinf(complex(%inf,%inf)) and isinf(complex(%nan,%inf)) returned %F
// =============================================================================
c = complex(%inf*[1 1 -1 -1], %inf*[1 -1 1 -1]);
assert_checktrue(and(isinf(c)));
c = complex([%nan %nan %inf -%inf], [%inf -%inf %nan %nan]);
assert_checktrue(and(isinf(c)));
