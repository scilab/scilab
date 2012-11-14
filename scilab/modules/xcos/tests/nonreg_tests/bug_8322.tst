// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8322 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8322
//
// <-- Short Description -->
// Rotated block had 2 different bounds (icons and port)

// xcos();
// Put a SUM_f block
// Resize it to have : width >> height
// Rotate it
// The icon *must* always be into the bounds

