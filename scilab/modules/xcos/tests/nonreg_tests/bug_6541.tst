// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 6541 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6541
//
// <-- Short Description -->
// This test validate Copy/Cut-n-Paste operation

// xcos()
// Add the sum block
// Rotate it (Ctrl-R)
// Copy (Ctrl-C)
// Paste (Ctrl-V)
// Check that the ports and block representation are the same between the 
// first and the pasted one.

