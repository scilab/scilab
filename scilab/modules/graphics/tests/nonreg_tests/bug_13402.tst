// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13402 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13402
//
// <-- Short Description -->
// Bounding boxes of xarcs were not correct

xarc(0, 0, 2, 1, 64 * 0, 64 * 360);
a = gca();
assert_checkequal(a.data_bounds, [0 -1 ; 2 0]);
clf();

xarc(0, 0, 2, 1, 64 * 0, 64 * 90);
a = gca();
assert_checkequal(a.data_bounds, [1 -0.5 ; 2 0]);
clf();

xarc(0, 0, 2, 1, 64 * 45, 64 * 90);
a = gca();
assert_checkalmostequal(a.data_bounds, [1-sqrt(2)/2 sqrt(2)/4-0.5 ; 1+sqrt(2)/2 0]);
clf();

xarc(0, 0, 2, 1, 64 * 45, 64 * -90);
a = gca();
assert_checkalmostequal(a.data_bounds, [1+sqrt(2)/2 -0.5-sqrt(2)/4 ; 2 sqrt(2)/4-0.5]);
clf();
