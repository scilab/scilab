// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10544 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10544
//
// <-- Short Description -->
// debug did not return the previous value when called with an argument.
//

A = debug(0);
assert_checkequal(A, 0);

B = debug(1);
assert_checkequal(B, 0);

C = debug(0);
assert_checkequal(C, 1);
