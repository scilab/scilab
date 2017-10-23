// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 5602 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/5602
//
// <-- Short Description -->
// Changing a component of a list in a cells array replaced the whole list with
// the insertion

c = {%i, list("ok", %z)};
c{1,2}(2) = %z^2;
assert_checkequal(c{1,2}, list("ok", %z^2));

assert_checkfalse(execstr("c(1,2)(2)", "errcatch")==0);
