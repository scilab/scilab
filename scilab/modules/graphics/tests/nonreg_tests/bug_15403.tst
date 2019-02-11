// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 15403 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15403
//
// <-- Short Description -->
// In "stacked" mode, bar(..) did not start from y=0

// The vertical bars must all start from 0
clf
subplot(1,3,1)
bar(2,[2 1 3],"stacked")
subplot(1,3,2)
bar(1:5, 1+rand(1,5), "stacked")
subplot(1,3,3)
bar(1:5, rand(1,5)-0.5, "stacked")

