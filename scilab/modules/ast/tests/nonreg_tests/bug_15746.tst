// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15746 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15746
//
// <-- Short Description -->
// 1/[1 2 3] and [1 2 3]'\1 should raise an error

B=1;
A=[1 2 3];
assert_checkerror("B/A", msprintf(_("Inconsistent row/column dimensions.\n")));
A=A';
assert_checkalmostequal((B/A)*A,B);
assert_checkerror("A\B", msprintf(_("Inconsistent row/column dimensions.\n")));
A=A';
assert_checkalmostequal(A*(A\B),B);

