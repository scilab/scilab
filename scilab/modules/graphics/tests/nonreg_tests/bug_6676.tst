// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 6676 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6676
//
// <-- Short Description -->
// When we executed "twinkle(gca())", data related to the current axes were not twinkling.

// When you execute this code, all figure should twinkle. Then, only bottom axes should twinkle.
clf
plot()
twinkle(gcf(),10)
sleep(2000)
twinkle(gca(),10)

