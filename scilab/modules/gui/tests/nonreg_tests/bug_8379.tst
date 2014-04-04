// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 8379 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8379
//
// <-- Short Description -->
// It was not possible to delete the selected datatip with DELETE or BACK_SPACE

plot2d(1:100);
e = gce();
e = e.children(1);
datatipCreate(e, 10);
datatipCreate(e, 30);
datatipCreate(e, 50);
datatipCreate(e, 70);

// Select datatips one by one and delete them in using the keys DELETE or BACK_SPACE
