// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16019 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16019
//
// <-- Short Description -->
// polarplot(x,Z) yielded an error when x is a vector and Z is a matrix.

x = (0:360)'/180*%pi;
Z = [(0.5*(1 + cos(x)))  (0.5+(1 + cos(x)))]; 
assert_checkequal(execstr("polarplot(x,Z)", "errcatch"), 0);
