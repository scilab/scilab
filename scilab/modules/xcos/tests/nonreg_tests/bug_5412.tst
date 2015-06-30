/ =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 5412 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5412
//
// <-- Short Description -->
//

// Start xcos
// Add a block (CONST_m for example)
// Select it --> Right click --> Selection to superblock
// Edit the superblock (double click on it)
// Simulation >> Set context and enter some text --> Click OK
// Check that the superblock is indicated as modified
// Close the superblock window

// Edit the superblock (double click on it)
// Simulation >> Set context
// Check that the text you entered is still there

