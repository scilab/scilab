// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 16358 -->
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16358
//
// <-- Short Description -->
// isdef([],..) yielded an error instead of returning []

assert_checkequal(isdef([]), []);
assert_checkequal(isdef([],"a"), []);
assert_checkequal(isdef([],"l"), []);
assert_checkequal(isdef([],"n"), []);
