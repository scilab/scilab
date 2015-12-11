// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11997 -->
// <-- CLI SHELL MODE -->
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11997
//
// <-- Short Description -->
// save function show compatibility warning for non existing variable instead of error

clear a;
//3 instead of 2 because of call through execstr
errmsg = msprintf(_("%s: Wrong value for input argument #%d: Defined variable expected.\n"), "save", 2);
assert_checkerror("save(TMPDIR + ""/myData.sod"", ""a"")", errmsg);

