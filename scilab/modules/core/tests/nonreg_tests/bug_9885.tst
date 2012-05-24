// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 9886 -->
//
// <-- CLI SHELL MODE -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9686
//
// <-- Short Description -->
// user initialization scripts (scilab.ini, .scilab) did not work.

txt = "BUG_9885_VARIABLE = 999;";
assert_checkequal(mputl("BUG_9885_VARIABLE = 999;" , SCIHOME + "/.scilab"), %t);

// restart scilab
assert_checkequal(isdef("BUG_9885_VARIABLE"), %t);
assert_checkequal(BUG_9885_VARIABLE, 999);

// remove .scilab
mdelete(SCIHOME + "/.scilab");
