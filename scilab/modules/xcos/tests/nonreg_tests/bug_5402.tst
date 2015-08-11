/ =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 5402 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5402
//
// <-- Short Description -->
// When "Save as" is canceled, status bar still displays "Saving diagram ...".

// start xcos

// File >> Save as >> Cancel

// Check that the status bar content is empty
