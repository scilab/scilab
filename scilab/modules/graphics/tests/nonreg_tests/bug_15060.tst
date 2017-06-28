// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15060 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15060
//
// <-- Short Description -->
// fplot3d did not draw because of an addition with an empty matrix which now 
// returns an empty matrix.

deff('z=f(x,y)','z=x^4-y^4')
x=-3:0.2:3 ;y=x ;
clf() ;fplot3d(x,y,f)
