// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15983 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15983
//
// <-- Short Description -->
// group() regressed in Scilab 5.5.2 due a too intrusive fix of bug 13692

h = wfir('lp',7,[.2,0],'hm',[0.01,-1]);
assert_checkequal(execstr("group(100,h)", "errcatch"), 0);

// As well:
// test_run signal_processing bug_13205 show_error
// test_run signal_processing bug_13692 show_error

