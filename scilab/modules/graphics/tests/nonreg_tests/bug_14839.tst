// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 14839 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14839
//
// <-- Short Description -->
// plot2d2 crashed Scilab.

t=linspace(0,10,61);
plot2d2(t,t);
