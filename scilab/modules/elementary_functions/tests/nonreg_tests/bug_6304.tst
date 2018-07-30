// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 6304 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6304
//
// <-- Short Description -->
// setdiff() returned %nan at the beginning

assert_checkequal(setdiff([%nan 3 %nan 4  ],[ 1 2 %nan 3 5 %inf]), [4 %nan %nan]);
