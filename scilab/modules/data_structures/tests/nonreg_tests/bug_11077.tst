// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 11077 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11077
//
// <-- Short Description -->
// rlist() returned an error

msg = msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"),"rlist",2,3);
assert_checkerror("rlist()", msg);
