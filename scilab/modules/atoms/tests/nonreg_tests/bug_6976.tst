// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 6976 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6976
//
// <-- Short Description -->
// Improve connection from the GUI to the related ATOMS webpages.
// In the Presentation frame: Add a "See also" or "Atoms webpage" section,
// giving the related hyperlink. Ex: For the Matrix Market module, give
// http://atoms.scilab.org/toolboxes/MatrixMarket
// Or set this hyperlink on the module's title.
// This would allow to easily access important infos that are presently not
// accessible through the GUI, such as comments,download rate, supported OS, etc.

atomsGui();

// Select a module
// Check the "See Also" section
