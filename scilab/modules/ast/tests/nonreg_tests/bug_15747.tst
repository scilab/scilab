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
// <-- Non-regression test for bug 15747 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15747
//
// <-- Short Description -->
// For t of type "test", 2.*t  calls %s_x_test() (OK) and then crashes Scilab (Regression)

t = tlist("test")
function %s_x_test(a,b)
    disp("%s_x_test")
endfunction
msg = sprintf(_("%s: Wrong number of arguments: %d expected while %d given.\n"), "%s_x_test", 0, 1);
assert_checkerror("2.*t", msg);
