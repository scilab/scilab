// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 6859 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6859
//
// <-- Short Description -->
// xlabel and xtitle could overlapped

clf();
plot2d()
xtitle("Titre principal","Légende axe X")
ca=gca();

// Observe that title and x label did not overlap
ca.x_location="top";