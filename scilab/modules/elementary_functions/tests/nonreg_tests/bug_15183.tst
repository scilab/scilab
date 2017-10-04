// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15183 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15183
//
// <-- Short Description -->
//  Contrarily to meshgrid(x) with y=x, ndgrid(x) was not accepted

[X, Y] = ndgrid(1:4);
assert_checkequal(Y, X.');
