// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug  -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9044
//
// <-- Short Description -->
// Updating a LaTeXed label of uimenu() misworks

m = uimenu(gcf(),"Label","ABC")  // See m.label in the listing, and the display: OK
m.label = "BCD"    // idem. OK. Normal strings work
m.label = "$x^2$" // => m.label should contain $x^2$
m.label = "$x^3$"  // => m.label should contain $x^3$
m.label = "ABC" // => m.label contains "ABC" and the displayed menu should be ABC