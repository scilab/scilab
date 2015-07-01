// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 13289 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13289
//
// <-- Short Description -->
// l=mlist(["A" "a"],1);l(rand(2,1))=0 lead to a segfault

assert_checkerror("l = mlist([""A"" ""a""], 1);l(rand(2,1)) = 0;", msprintf(_("Function not defined for given argument type(s),\n") + "%s", msprintf(_("  check arguments or define function %s for overloading.\n"), "%s_i_A")));