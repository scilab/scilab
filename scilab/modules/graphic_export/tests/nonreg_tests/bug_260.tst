// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 260 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=260
//
// <-- Short Description -->
// When setting the property "auto_ticks" of the Axes, it is not set properly.


// create an acentued text
xstring(0, 0, "totoééééààààààçççç@@@@@@@");

// the exported file should display the above string with accents.
xs2eps(0, TMPDIR+"/toto.eps");

