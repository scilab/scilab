// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 10278 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10278
//
// <-- Short Description -->
// clear(1) returned a wrong error message

ierr = execstr("clear(1)", "errcatch");
assert_checkequal(ierr, 207);
msg_ref = msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "clear", 1);
assert_checkerror ("clear(1)", msg_ref);
