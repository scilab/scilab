// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 5611 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/5611
//
// <-- Short Description -->
// It was not possible to delete a row or column of a cell array

c0 = {"Hello", %pi, 1.2 ; %t, %z, "abc" ; cos, %i, list(1,%f)};
c = c0;
assert_checkfalse(execstr("c(2,:) = null()", "errcatch")==0);
assert_checktrue(execstr("c(2,:) = []", "errcatch")==0);
assert_checkequal(c, {"Hello", %pi, 1.2; cos, %i, list(1,%f)});

c = c0;
assert_checkfalse(execstr("c(:,2) = null()", "errcatch")==0);
assert_checktrue(execstr("c(:,2) = []", "errcatch")==0);
assert_checkequal(c, {"Hello", 1.2; %t, "abc" ; cos, list(1,%f)});
