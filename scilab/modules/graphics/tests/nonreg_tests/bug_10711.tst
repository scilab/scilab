// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 10711 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10711
//
// <-- Short Description -->
// the textbox of datatip is not displayed with grahs using logarithm scale

plot2d([1 2 3 4], [1 2 3 4]);
a = gca();

// Set a datatip on the curve.
a.log_flags = "lln";

// Check that the datatip label is displayed.
