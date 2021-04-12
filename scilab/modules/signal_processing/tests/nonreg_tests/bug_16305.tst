// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 16305 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16305
//
// <-- Short Description -->
// unwrap(,"unfold") failed unfolding straight segments

x = linspace(0,10,1000);
y = acos(cos(x));
r = unwrap(y,'unfold');

Ref = [0. 1.2412412 2.4824825 3.7190723 4.9603135 6.2015547 7.436594 8.6778353 9.9051221];
assert_checkalmostequal(r(1:124:1000), Ref, 1e-7);
