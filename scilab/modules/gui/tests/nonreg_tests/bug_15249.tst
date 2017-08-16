// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15249 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15249
//
// <-- Short Description -->
// findobj("toto") yielded an error instead of returning []

clf
assert_checkequal(findobj("toto"), []);
