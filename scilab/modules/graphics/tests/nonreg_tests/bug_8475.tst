// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8475 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8475
//
// <-- Short Description -->
//
// It was impossible to get the data of an entity through the graphic editor.
//

plot()
ged(9,0)

// In the GED :
// Select a polyline.
// Select the 'Data' tab.
// In the 'Data field' menu, select "Edit data .."
// you should edit data