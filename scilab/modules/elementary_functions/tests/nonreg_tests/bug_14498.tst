// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14498 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14498
//
// <-- Short Description -->
// size([],3) returned 1 instead of 0

assert_checkequal(size([],3), 0)
