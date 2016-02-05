// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13321 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13321
//
// <-- Short Description -->
// scf() did not create an invisible figure when the gdf.visible property was set to "off".

f=gdf();
f.visible="off";
g=figure()
// Check that the visible property of g is set to "off"
// Check that no figure is visible

g.visible="on";
// Check that the figure is now visible