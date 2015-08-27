//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3687 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3687
//
// <-- Short Description -->
// "format" doesn't check input value 

fmt = format();
assert_checkequal(fmt, [1 10]);
format(fmt(2),fmt(1));
fmt2 = format();
assert_checkequal(fmt, fmt2);
msgerr = msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the interval [%d, %d].\n"),"format",1,2,25);
assert_checkerror ("format(0)", msgerr);
