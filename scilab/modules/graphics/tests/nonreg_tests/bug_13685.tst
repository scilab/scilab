// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13685 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13685
//
// <-- Short Description -->
// LaTeX was not detected when specified in ticks_format.

clf();
plot2d();

// ticks don't use LaTeX
a = gca();

// ticks now use LaTeX
a.ticks_format=["$%.1f$","$%.1f$",""];
