// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 11629 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11629
//
// <-- Short Description -->
// Interactive zoom misworked in datatip mode.

plot2d(1:100);
e = gce();
e = e.children(1);
datatipCreate(e, 10);
datatipCreate(e, 70);

// Now play the interactive zoom
// Put zoom edge on the plot or on the datatips