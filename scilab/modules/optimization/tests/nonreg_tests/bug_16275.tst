// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Alain LAMY
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16275 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16275
//
// <-- Short Description -->
// fsolve(x0, fun, tol) no longer took tol into account

function y = f(x)
  y = x * x * x - 8; 
endfunction

[x1, y1] = fsolve(1, f);
[x2, y2] = fsolve(1, f, 0.1);

assert_checktrue(x1 <> x2);
assert_checktrue(y1 <> y2);
