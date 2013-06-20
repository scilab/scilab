// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 12686 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12686
//
// <-- Short Description -->
// diff() with zero input arguments yielded the wrong error

// Run with test_run('linear_algebra', 'bug_12686', ['no_check_error_output'])

refMsg = msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), "diff", 1, 3);
assert_checkerror("diff()", refMsg);
