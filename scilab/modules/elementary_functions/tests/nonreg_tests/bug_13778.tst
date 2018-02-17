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
// <-- Non-regression test for bug 13778 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13778
//
// <-- Short Description -->
// size(repmat([],[1 1 3]) was [0 0 3] insted of [0 0]

assert_checkequal(repmat([], [1 1 3]),[]);
assert_checkequal(size(repmat([], [1 1 3])),[0 0]);

