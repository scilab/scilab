// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 14692 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14692
//
// <-- Short Description -->
//    isequal() was always returning True for built-in functions

tested_fun = sin; // sin is a builtin
tested_fun2 = cos; // cos is a builtin

assert_checktrue(isequal(tested_fun, sin));
assert_checktrue(isequal(tested_fun2, cos));

assert_checkfalse(isequal(tested_fun, cos));
assert_checkfalse(isequal(tested_fun2, sin));
assert_checkfalse(isequal(tested_fun, isempty)); // isempty is a macro
assert_checkfalse(isequal(tested_fun2, isempty)); // isempty is a macro

assert_checktrue(tested_fun == sin);
assert_checktrue(tested_fun2 == cos);
assert_checktrue(tested_fun <> cos);
assert_checktrue(tested_fun2 <> sin);

