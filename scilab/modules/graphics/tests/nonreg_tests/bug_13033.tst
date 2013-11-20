// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13033 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13033
//
// <-- Short Description -->
// -1 can be used as major number of ticks in plot2d

clf;
x=0:0.1:5;
// Following line produced a critical error before this bug fix.
plot2d(x,sin(x),nax=[2,-1,2,-1]);