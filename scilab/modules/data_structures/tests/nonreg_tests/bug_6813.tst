// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 6813 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6813
//
// <-- Short Description -->
// When creating a N>2-D hyperarray of cells, makecell() yielded an error

assert_checktrue(execstr("makecell([2,2,2], 1,2,3,4,5,6,7,8)", "errcatch")==0);
assert_checktrue(execstr("makecell([1,2,2,2], 1,2,3,4,5,6,7,8)", "errcatch")==0);
assert_checktrue(execstr("makecell([1,1,2,2,2], 1,2,3,4,5,6,7,8)", "errcatch")==0);
assert_checktrue(execstr("makecell([2,2,2,2,2], "+strcat(string(1:32),", ")+")", "errcatch")==0);
