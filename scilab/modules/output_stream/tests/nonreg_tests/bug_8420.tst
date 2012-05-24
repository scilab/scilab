// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 8420 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8420
//
// <-- Short Description -->
// 'lines' crashed scilab in some cases
//

msgerr = msprintf(gettext("%s: Wrong value for input argument #%d: A scalar (>= 0) expected.\n"), "lines", 2);
assert_checkerror ( "lines(0, 0)" , msgerr );

// lines test requires interaction here
lines(10,10);
rand(10,10)

