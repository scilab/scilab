// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13365 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13365
//
// <-- Short Description -->
// Data bounds not correctly updated in 3D

scf();
param3d([0,1], [0,1], [0,1]);
xsegs([2;3], [2;3], [2;3], 5);
a=gca();

assert_checkequal(a.data_bounds, [0 0 0; 3 3 3]);