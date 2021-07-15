// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16340 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16340
//
// <-- Short Description -->
// setdiff("", "") yielded "" instead of []

assert_checkequal(setdiff("",""), []);
assert_checkequal(setdiff("",[]), "");
assert_checkequal(setdiff([],""), []);
assert_checkequal(setdiff("",["1" "2" ""]), []);
