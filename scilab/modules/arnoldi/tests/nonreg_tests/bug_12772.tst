// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 12772 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12772
//
// <-- Short Description -->
//    eigs() failed when trying to solve a sparse matrix eigen value problem.
// =============================================================================

C1 = -51*eye(10,10);
C1(1:$-1, 2:$) = C1(1:$-1,2:$) + diag(100*ones(9,1));
C1(2:$,1:$-1) = C1(2:$,1:$-1) + diag(100*ones(9,1));
C1(10,9) = 116;
C1(10,10) = -147;
C1 = sparse(C1);

[d, v] = eigs(C1,sparse(eye(10,10)), 1, 480);
assert_checkalmostequal(C1*v, v*d, sqrt(%eps), 1.e-10);
