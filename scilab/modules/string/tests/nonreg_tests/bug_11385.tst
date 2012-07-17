// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 11385 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11385
//
// <-- Short Description -->
// Wrong error message returned by length when called without input argument.
//

refMsg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "length", 1);
assert_checkerror("length()", refMsg);

