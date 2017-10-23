// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15246 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15246
//
// <-- Short Description -->
//    chsolve() called a missing blkslv() macro, that called a missing blkslvi() builtin

// From chsolve() example:
A = sprand(20,20,0.1);
A = A*A'+eye();
spcho = chfact(A);
sol = (1:20)';
rhs = A*sol;
spcho = chfact(A);
chsolve(spcho,rhs);
