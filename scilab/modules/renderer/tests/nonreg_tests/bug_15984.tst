// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- MACOSX ONLY-->
// <-- NO CHECK REF-->
//
// <-- Non-regression test for bug 15984 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15984
//
// <-- Short Description -->
// display scale is wrong for Retina displays under OSX

plot(1:10,sin(1:10))
ax = gca();
assert_checkequal(ax.x_ticks.locations,(1:10)');
