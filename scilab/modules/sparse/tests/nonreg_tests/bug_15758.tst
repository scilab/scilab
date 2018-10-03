// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15758 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15758
//
// <-- Short Description -->
// sparse([0 0],1) crashes scilab (regression)

msg = sprintf(_("%s: Invalid index.\n"), "sparse");
assert_checkerror("sparse([0 0],1,[1 1])", msg);
assert_checkerror("sparse([0 1],1,[1 1])", msg);
assert_checkerror("sparse([1 0],1,[1 1])", msg);
assert_checkerror("sparse([2 1],1,[1 1])", msg);
assert_checkerror("sparse([1 2],1,[1 1])", msg);
assert_checkerror("sparse([2 2],1,[1 1])", msg);
