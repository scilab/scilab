// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15299 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15299
//
// <-- Short Description -->
// plot() silently failed plotting polynomials or rationals

s = %s;

// Polynomials
xp = -0.5:0.02:3;
p = s*[2 ; (s-2) ; (s-1)*(s-2)];
assert_checkequal(execstr("plot(xp, p)","errcatch"), 0);

// Rationals
xr = -0.5:0.02:2;
r = (s-1)*s/(s-7)./[s+1, s-4];
clf
assert_checkequal(execstr("plot(xr, r)","errcatch"), 0);

// Both
clf
assert_checkequal(execstr("plot(xp, p, xr, r)","errcatch"), 0);
