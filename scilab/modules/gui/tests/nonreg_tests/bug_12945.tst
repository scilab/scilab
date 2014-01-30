// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 12945 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12945
//
// <-- Short Description -->
// Datatips could not correctly dragged in zoomed axes

scf();
plot2d();
e = gce();
e = e.children(1);
datatipCreate(e, [2.2 0.2]);
zoom_rect([2,0,3,1]);

// Enable datatip mode and drag the datatip
// the graphic translation must be disabled

