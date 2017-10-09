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
// <-- Non-regression test for bug 13979 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13979
//
// <-- Short Description -->
// a variable whose name is longer than 24 characters could not be cleared

aaaaaaaa10bbbbbbbb20cccccccc30 = 1;
clear aaaaaaaa10bbbbbbbb20cccccccc30
assert_checkfalse(isdef("aaaaaaaa10bbbbbbbb20cccccccc30","l"));
