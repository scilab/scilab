// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 16158 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16158
//
// <-- Short Description -->
// When a multicolumn array of rationals was displayed column per column,
// columns #2:$ were replaced with its column #2

p = poly(ones(1,15),"x","coeff");
R = [1 2 3] ./ p
R = [1;1]*[1 2 3] ./ p

p = poly(ones(1,7),"x","coeff");
R = [1 2 3] ./ p
R = [1;1]*[1 2 3] ./ p
