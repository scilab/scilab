// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13517 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13517
//
// <-- Short Description -->
// isdef fails with a vector of strings when she is used in a function and after
// a declaration of variable.

function bug_13517(str)
    vn = 1;
    isdef(str)
endfunction

ierr=execstr("bug_13517(""a"")", "errcatch");
assert_checktrue(ierr==0);

ierr=execstr("bug_13517([""a"" ""b""])", "errcatch");
assert_checktrue(ierr==0);
