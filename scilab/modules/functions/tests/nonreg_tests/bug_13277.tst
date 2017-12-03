// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 13277 -->
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13277
//
// <-- Short Description -->
// execstr('clear') failed clearing variables of the current scope

x = 5;
execstr("clear");
assert_checktrue(~isdef("x","l"));
