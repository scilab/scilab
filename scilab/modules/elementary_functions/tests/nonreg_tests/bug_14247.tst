// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 14247-->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14247
//
// sqrt function did not work on hypermatrices.

a(1:2,1:2,1:2)=0;
a(:)=(1:8).^2;
res = sqrt(a);
expected = matrix(1:8, [2 2 2]);
assert_checkequal(res, expected);
