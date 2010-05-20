// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4117 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4117
//
// <-- Short Description -->
// Closing a graphics figure (via the GUI) which has a non-existing
// event handler attached does not destroy the figure's associated data structure.
//

fig = scf(0);
fig.event_handler = "evxxxx"
fig.event_handler_enable = "on"

// Close the figure using the window close button or the File->close menu,
// this should destroy the figure.

// Check that the closed figure's handle "fig" is not valid anymore.

