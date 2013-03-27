// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
// Copyright (C) 2013 - Scilab Enterprises - Calixte Denizet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 12405 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12405
//
// <-- Short Description -->
// Floating point errors could lead to an infinite loop in ticks computation.

plot2d([1-1e-16 1])
