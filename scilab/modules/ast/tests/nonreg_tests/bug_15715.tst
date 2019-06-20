// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15715 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15715
//
// <-- Short Description -->
// Referencing a 2D array with one valid index and %nan for the second index causes a crash

A = eye(3,3);
message = msprintf(gettext("Invalid index.\n"));

assert_checkerror("A(%nan,1)",message)
assert_checkerror("A(%nan,%nan)",message)
assert_checkerror("A(1,%nan)",message)
assert_checkerror("A(%nan,%nan)=1",message)
assert_checkerror("A(%nan,1)=1",message)
assert_checkerror("A(1,%nan)=1",message)

