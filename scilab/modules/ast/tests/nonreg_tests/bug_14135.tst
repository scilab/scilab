// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14135 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14135
//
// <-- Short Description -->
// crash when running "Graphics -> Matplot -> Java Image" demonstration.
// argument of a "function call"/"extraction" must be treated before the function/variable.

expected = msprintf(gettext("Undefined variable: %s\n"), "b");
assert_checkerror("a(b)",expected);
